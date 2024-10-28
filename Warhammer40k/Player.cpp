#include "Player.h"
#include <OgreSceneQuery.h>

#include "QueryFlags.h"
#include "CameraComponent.h"
#include "Figurines.h"
#include "Grid.h"
#include "OgreText.h"

#include <iostream>

Player::Player(GameEngine &gameEngineP, OverlayManager &overlayManagerP) :
	mGameEngine (gameEngineP),
    m_OverlayManager(overlayManagerP)
{
    /*  Add Components */
	CameraComponent *camera = new CameraComponent(gameEngineP);
    AddComponent(camera);

    mCamera = &camera->getCamera();
    gameEngineP.getRenderWindow()->addViewport(mCamera);

    /* Create the Ray */
    mRayScnQuery = gameEngineP.GetSceneManager()->createRayQuery(Ogre::Ray());
    mRayScnQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

    /* Add listener */
    gameEngineP.addInputListener(this);
    gameEngineP.SetPlayer(this);

    /* Create the Dice */
    //mDice = mGameEngine.GetSceneManager()->createEntity("Dice", "Dice.mesh");
    //mDice->setMaterialName("Dice");
    //SceneNode* diceNode = mGameEngine.GetSceneManager()->getRootSceneNode()->createChildSceneNode("DiceNode");
    //diceNode->attachObject(mDice);
    //diceNode->setPosition(Vector3(0, 10, 0));
    //diceNode->setScale(Vector3(.05f, .05f, .05f));
}

Player::~Player()
{
    mGameEngine.GetSceneManager()->destroyQuery(mRayScnQuery);
}

void Player::Update(float deltaTimeP)
{
	Actors::Update(deltaTimeP);
}

void Player::OnEndTurn()
{
    SwapPlayer();
    UnselectFigurine();
    ResetMouseOver();
}

void Player::MouseRayTo3D(int mouseXP, int mouseYP)
{
    /* Cast Ray */
    float width = mouseXP / (float)mGameEngine.getRenderWindow()->getWidth();
    float height = mouseYP / (float)mGameEngine.getRenderWindow()->getHeight();
    
    mMouseRay = mCamera->getCameraToViewportRay(width, height);
    
    mRayScnQuery->setRay(mMouseRay);
    mRayScnQuery->setSortByDistance(true);
    mRayScnQuery->setQueryMask(FIGURINE_MASK | OBSTACLE_MASK);
    
    CheckMouseOverCollisions();
}

void Player::CheckMouseOverCollisions()
{
    RaySceneQueryResult& result = mRayScnQuery->execute();
    RaySceneQueryResult::iterator it = result.begin();

    if (it == result.end() || !it->movable || it->movable->getQueryFlags() != FIGURINE_MASK)
    {
        ResetMouseOver();
        return;
    }

    /* Try to get the entity's actor class and cast it to figurines */
    SceneNode* sceneNodeHit = it->movable->getParentSceneNode();
    Actors* getActor = mGameEngine.GetSceneActor(sceneNodeHit);
    if (auto* newTarget = dynamic_cast<Figurines*>(getActor))
    {
        if (newTarget != mCurrentMouseOver)
        {
            HandleMouseOver(newTarget);
        }
    }
    else
    {
        ResetMouseOver();
    }
}

void Player::HandleMouseOver(Figurines* mouseOverTargetP)
{
    ResetMouseOver();

    if (!mouseOverTargetP->IsSleeping()) return;
    mCurrentMouseOver = mouseOverTargetP;

    bool isEnemy = mCurrentMouseOver->GetOwner() != mCurrentPlayerID;
    mCurrentMouseOver->OnMouseOver(isEnemy);
}

void Player::ResetMouseOver()
{
    if (mCurrentMouseOver)
    {
        mCurrentMouseOver->OnMouseOut();
    }

    mCurrentMouseOver = nullptr;
}

void Player::OnLBMouseDown(int mouseXP, int mouseYP)
{
    /* Unselect current selection if nothing is Moused Over */
    if (!mCurrentMouseOver)
    {
        UnselectFigurine();
        return;
    }

    /* Check if selecting an Enemy's figurine */
    if (mCurrentMouseOver->GetOwner() != mCurrentPlayerID)
    {
        mCurrentEnemyTargeted = mCurrentMouseOver;
        ShowFigurineCard(*mCurrentEnemyTargeted, true);
    }
    else
    {
        HandleFigurineSelection();
    }
}

void Player::HandleFigurineSelection()
{
    /* If no current selection, select the new figurine */
    if (!mIsFigurineSelected)
    {
        SelectPlayerFigurine(*mCurrentMouseOver);
        return;
    }
    else if (mCurrentMouseOver->GetSceneNode() != mCurrentPlayerFigurine->GetSceneNode())
    {
        UnselectFigurine();
        SelectPlayerFigurine(*mCurrentMouseOver);
    }
    else
    {
        UnselectFigurine();
    }
}

void Player::SelectPlayerFigurine(Figurines& figurineP)
{
    mCurrentPlayerFigurine = &figurineP;
    mCurrentPlayerFigurine->OnSelected(true);
    mIsFigurineSelected = true;

    ShowFigurineCard(figurineP, false);
}

void Player::UnselectFigurine()
{
    // Unselect the last selected actor
    if (mCurrentPlayerFigurine)
    {
        mCurrentPlayerFigurine->OnSelected(false);
        mCurrentPlayerFigurine = nullptr;
    }

    mCurrentEnemyTargeted = nullptr;
    mIsFigurineSelected = false;

    HideCards();
}

void Player::OnRBMouseDown(int mouseXP, int mouseYP)
{
    /* Check if a figurine is selected or already doing an action */
    if (!mCurrentPlayerFigurine || !mCurrentPlayerFigurine->IsSleeping()) return;

    CheckRightClickCollisions();
}

void Player::CheckRightClickCollisions()
{
    /* Cast a Ray to check what we right click on */
    RaySceneQueryResult& result = mRayScnQuery->execute();
    RaySceneQueryResult::iterator hitResult = result.begin();
    if (hitResult == result.end()) return;

    if (hitResult->movable->getQueryFlags() == QueryFlags::FIGURINE_MASK)
    {
        HandleFigurineTargeting(hitResult);
    }
    else if (hitResult->movable->getQueryFlags() != FIGURINE_MASK && hitResult->movable->getQueryFlags() != OBSTACLE_MASK)
    {
        HandleFigurineMovement(hitResult);
    }
}

void Player::HandleFigurineTargeting(Ogre::RaySceneQueryResult::iterator& hitResult)
{
    /* Check if the selected figurine has any ActionPoint Left*/
    if (mCurrentPlayerFigurine->GetActionPoints() <= 0) return;

    /* Return if targeting the figurine that is selected */
    if (mCurrentPlayerFigurine->GetEntity() == hitResult->movable) return;

    /* Try to get the entity's actor class and cast it to figurines */
    SceneNode* sceneNodeHit = hitResult->movable->getParentSceneNode();
    Actors* getActor = mGameEngine.GetSceneActor(sceneNodeHit);
    if (auto* newTarget = dynamic_cast<Figurines*>(getActor))
    {
        /* Check if targeting an ally */
        if (newTarget->GetOwner() == mCurrentPlayerFigurine->GetOwner()) return;
        // mCurrentPlayerFigurine->HealOrBuff(newTarget)                                         // TODO 

        /* Check if the enemy is On Shooting Sight */
        float distanceFromSelected = 0.f;                                                  // TODO: IMPLEMENT DISTANCE CALCULATION
        if (distanceFromSelected > mCurrentPlayerFigurine->GetAttackRange()) return;
        mCurrentPlayerFigurine->Attack(newTarget);

        UpdateCardText(*mCurrentPlayerFigurine, false);

        if (mCurrentEnemyTargeted == newTarget)
        {
            UpdateCardText(*mCurrentEnemyTargeted, true);
        }
    }
}

void Player::HandleFigurineMovement(Ogre::RaySceneQueryResult::iterator& hitResult)
{
    /* Calculate the target position depending on the hitresult */
    Vector3 rayOrigin = mMouseRay.getOrigin();
    Vector3 rayDirection = mMouseRay.getDirection();

    Vector3 targetPosition;
    targetPosition.x = rayOrigin.x + hitResult->distance * rayDirection.x;
    targetPosition.y = rayOrigin.y + hitResult->distance * rayDirection.y;
    targetPosition.z = rayOrigin.z + hitResult->distance * rayDirection.z;

    Tile* targetTile = mGameEngine.GetGrid().GetTile(targetPosition);
    if (!targetTile) return;

    /* Check if the tile is on the Figurine Grid Movement */
    if (targetTile->GetType() == TILE_MOVEMENT_SELECTED)
    {
        targetPosition.y = 2.f;

        /* Move to the target position */
        mCurrentPlayerFigurine->MoveTo(targetTile);
        UpdateCardText(*mCurrentPlayerFigurine, false);
    }
}

void Player::UpdateCardText(const Figurines &figurineP, bool isRightCardP)
{
    /* Update Left Card Text */
    float movementAction = figurineP.GetMovementAction();
    int healthPoint = figurineP.GetHealthPoints();
    int actionPoint = figurineP.GetActionPoints();

    std::string result = std::to_string(movementAction);
    result = result.substr(0, result.find('.')); // Remove Decimals

    std::string figurinesPoints =
        std::string("Health Points: " + std::to_string(healthPoint)) + " \n" +
        std::string("Action Points: " + std::to_string(actionPoint)) + " \n" +
        std::string("Movement Points: " + result
        );

    if (!isRightCardP)
    {
        m_OverlayManager.getOverlayElement("LeftCardText")->setCaption(figurinesPoints);
        return;
    }

    if (healthPoint <= 0)
    {
        m_OverlayManager.getOverlayElement("RightCard")->hide();
    }
    else
    {
        m_OverlayManager.getOverlayElement("RightCardText")->setCaption(figurinesPoints);
    }
}

void Player::ShowFigurineCard(const Figurines &figurineP, bool isRightCardP)
{
    /* Update the Card Texts */
    UpdateCardText(figurineP, isRightCardP);

    const std::string& figurineName = figurineP.GetEntity()->getName();

    /* Show right card if it's an enemy card */
    if (isRightCardP)
    {
        m_OverlayManager.getOverlayElement("RightCardNameText")->setCaption(figurineName);
        m_OverlayManager.getOverlayElement("RightCard")->show();
    }
    else
    {
        /* Show left card if it's a player card */
        m_OverlayManager.getOverlayElement("LeftCardNameText")->setCaption(figurineName);
        m_OverlayManager.getOverlayElement("LeftCard")->show();
    }
}

void Player::SetCardTextValues(float movementPointP, int healthPointP, bool isRightCardP)
{
    std::string result = std::to_string(movementPointP);
    result = result.substr(0, result.find('.')); // Remove Decimals

    std::string figurinesPoints = 
        std::string("Health Point: " + std::to_string(healthPointP)) + " \n" + 
        std::string("Health Point: " + std::to_string(healthPointP)) + " \n" + 
        std::string("Movement Point: " + result)
        ;
   
    if (isRightCardP)
    {
        m_OverlayManager.getOverlayElement("RightCardText")->setCaption(figurinesPoints);
    }
    else
    {
        m_OverlayManager.getOverlayElement("LeftCardText")->setCaption(figurinesPoints);
    }  
}

void Player::HideCards()
{
    m_OverlayManager.getOverlayElement("LeftCard")->hide();
    m_OverlayManager.getOverlayElement("RightCard")->hide();
}

bool Player::mouseMoved(const MouseMotionEvent& evt)
{
    if (mGameEngine.isGameLoaded)
    {
        MouseRayTo3D(evt.x, evt.y);
    }

    return false;
}

bool Player::mousePressed(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        OnLBMouseDown(evt.x, evt.y);
        mIsLMBDown = true;
    }

    if (key == 3)
    {
        OnRBMouseDown(evt.x, evt.y);
        mIsRMBDown = true;
    }

    return false;
}

bool Player::mouseReleased(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        mIsLMBDown = false;
    }

    if (key == 3)
    {
        mIsRMBDown = false;
    }

    return false;
}
