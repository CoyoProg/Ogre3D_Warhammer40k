#include "Player.h"

#include <OgreSceneQuery.h>
#include "QueryFlags.h"

#include "CameraComponent.h"
#include "Figurines.h"
#include "Grid.h"
#include "OgreText.h"

#include <iostream>

Player::Player(GameEngine &gameEngineP) :
	mGameEngine (gameEngineP),
    mRayScnQuery(0)
{
    /*  Add Components */
	CameraComponent *camera = new CameraComponent(gameEngineP);
	gameEngineP.getRenderWindow()->addViewport(camera->getCamera());
	AddComponent(camera);

    /* Create the Ray */
    mRayScnQuery = gameEngineP.GetSceneManager()->createRayQuery(Ogre::Ray());
    mRayScnQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

    /* Add listener */
    gameEngineP.addInputListener(this);
    gameEngineP.SetPlayer(this);

    /* Get Overlay References */
    m_OverlayManager = OverlayManager::getSingletonPtr();

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

    /* We dont want it to be called every framed */
    //mFunctionDelay -= deltaTime;
    //if (mCurrentSelected && mFunctionDelay <= 0)
    //{
    //    mFunctionDelay = .2f;
    //    IsOnMovementSight();
    //}
}

void Player::MouseRayTo3D(int mouseXP, int mouseYP)
{
    /* Cast Ray */
    float width = mouseXP / (float)mGameEngine.getRenderWindow()->getWidth();
    float height = mouseYP / (float)mGameEngine.getRenderWindow()->getHeight();

    CameraComponent *camComp = (CameraComponent*)myComponents[0];
    Camera *mCamera = camComp->getCamera();

    mMouseRay = mCamera->getCameraToViewportRay(width, height);

    mRayScnQuery->setRay(mMouseRay);
    mRayScnQuery->setSortByDistance(true);
    mRayScnQuery->setQueryMask(FIGURINE_MASK | OBSTACLE_MASK);


    // Check for Collisions //
    RaySceneQueryResult &result = mRayScnQuery->execute();
    RaySceneQueryResult::iterator it = result.begin();

    /* Check if the entity is not a figurine */
    if (!it->movable || it->movable->getQueryFlags() != FIGURINE_MASK)
    {
        ResetMouseOver();
        return;
    }

    /* Try to get the entity's actor class and cast it to figurines */
    SceneNode *sceneNodeHit = it->movable->getParentSceneNode();
    Actors *getActor = mGameEngine.GetSceneActor(sceneNodeHit);
    if (!getActor)
    {
        ResetMouseOver();
        return;
    }

    Figurines *temporary = dynamic_cast<Figurines*>(getActor);
    if (!temporary)
    {
        ResetMouseOver();
        return;
    }

    if (temporary != mCurrentMouseOver)
    {
        ResetMouseOver();

        mCurrentMouseOver = temporary;
        if (!mCurrentMouseOver->IsSleeping()) return;
        
        bool isEnemy = mCurrentMouseOver->GetOwner() != mPlayerID;
        mCurrentMouseOver->OnMouseOver(isEnemy);
    }
}

void Player::ResetMouseOver()
{
    if (mCurrentMouseOver)
        mCurrentMouseOver->OnMouseOut();

    mCurrentMouseOver = nullptr;
}

void Player::OnLBMouseDown(int mouseXP, int mouseYP)
{
    /* Unselect current selection if nothing is Moused Over */
    if (!mCurrentMouseOver && mIsActorSelected)
    {
        UnselectFigurine();
        return;
    }

    if (!mCurrentMouseOver)
    {
        HideCards();
        return;
    }

    /* Check if selecting an Enemy's figurine */
    if (mCurrentMouseOver->GetOwner() != mPlayerID)
    {
        ShowFigurineCard(mCurrentMouseOver, true);
        return;
    }

    /* If no current selection, select the new figurine */
    if (!mIsActorSelected)
    {
        SelectFigurine(mCurrentMouseOver);
        return;
    }

    /* Check if current selection isn't the same as the new selected figurine */
    if (mCurrentMouseOver->GetSceneNode() != mCurrentSelected->GetSceneNode())
    {
        UnselectFigurine();
        SelectFigurine(mCurrentMouseOver);
    }else
        UnselectFigurine();
}

void Player::OnRBMouseDown(int mouseXP, int mouseYP)
{
    /* Check if a figurine is selected */
    if (mCurrentSelected)
    { 
        /* Check if the figurine is already doing an action */
        if (!mCurrentSelected->IsSleeping())
            return;

        /* Cast a Ray to check what we right click on */
        RaySceneQueryResult &result = mRayScnQuery->execute();
        RaySceneQueryResult::iterator hitResult = result.begin();

        for (; hitResult != result.end(); hitResult++)
        {
            /* Check if the RayCast collide with a Figurine or an Obstacle */
            if (hitResult->movable->getQueryFlags() != FIGURINE_MASK && hitResult->movable->getQueryFlags() != OBSTACLE_MASK)
            {
                /* Calculate the target position depending on the hitresult */
                Vector3 rayOrigin = mMouseRay.getOrigin();
                Vector3 rayDirection = mMouseRay.getDirection();

                mNewPosition.x = rayOrigin.x + hitResult->distance * rayDirection.x;
                mNewPosition.y = rayOrigin.y + hitResult->distance * rayDirection.y;
                mNewPosition.z = rayOrigin.z + hitResult->distance * rayDirection.z;

                mTargetPosition = mNewPosition;
                Tile *targetTile = mGameEngine.GetGrid().GetTile(mTargetPosition);

                /* Check if the tile is on the Figurine Grid Movement */
                if (targetTile->GetType() == TILE_MOVEMENT_SELECTED)
                {
                    mTargetPosition.y = 2.f;

                    /* Move to the target position */
                    mCurrentSelected->MoveTo(targetTile);
                }

                /* Update Left Card Text */
                float movementAction = mCurrentSelected->GetMovementAction();
                int healthPoint = mCurrentSelected->GetHealthPoints();
                SetCardTextValues(movementAction, healthPoint);

                break;
            }

            /* Check if the selected figurine has any ActionPoint Left*/
            if (mCurrentSelected->GetActionPoints() <= 0)
                return;

            if(hitResult->movable->getQueryFlags() == QueryFlags::FIGURINE_MASK)
            {
                /* If right clicking on the current selected figurine, return */
                if (mCurrentSelected->GetEntity() == hitResult->movable)
                    return;


                /* Try to get entity's actor class and cast it to figurines */
                SceneNode *sceneNodeHit = hitResult->movable->getParentSceneNode();
                Actors *getActor = mGameEngine.GetSceneActor(sceneNodeHit);
                if (!getActor)
                    return;

                Figurines *target = dynamic_cast<Figurines*>(getActor);
                if (!target)
                    return;

                /* Check if targeting an ally */
                if (target->GetOwner() == mCurrentSelected->GetOwner())
                    return; // Heal or Buff Ally Here

                /* Check if the enemy is On Shooting Sight */
                if (mDistanceFromSelected > mCurrentSelected->GetAttackRange())
                    return;

                mCurrentSelected->Attack(target);
            }
            break;
        }
    }
}

void Player::ShowFigurineCard(Figurines *figurineP, bool isRightCardP)
{
    /* Update the Card Texts */
    float movementAction = figurineP->GetMovementAction();
    int healthPoint = figurineP->GetHealthPoints();
    SetCardTextValues(movementAction, healthPoint, isRightCardP);

    std::string figurineName = figurineP->GetEntity()->getName();

    /* Show right card if it's an enemy card */
    if (isRightCardP)
    {
        m_OverlayManager->getOverlayElement("RightCardNameText")->setCaption(figurineName);
        m_OverlayManager->getOverlayElement("RightCard")->show();

        return;
    }

    /* Show left card if it's a player card */
    m_OverlayManager->getOverlayElement("LeftCardNameText")->setCaption(figurineName);
    m_OverlayManager->getOverlayElement("LeftCard")->show();
}

void Player::SetCardTextValues(float movementPointP, int healthPointP, bool isRightCardP)
{
    // Convert to string with precision of 1 decimal place
    std::string result = std::to_string(movementPointP);
    size_t dotPos = result.find('.');
    if (dotPos != std::string::npos && result.length() > dotPos + 2)
    {
        result = result.substr(0, dotPos + 2);
    }

    std::string figurinesPoints = std::string("Health Point: " + std::to_string(healthPointP) + " \n\n" + std::string("Movement Point: ") + result);
   
    if (isRightCardP)
    {
        m_OverlayManager->getOverlayElement("RightCardText")->setCaption(figurinesPoints);
        return;
    }
    
    m_OverlayManager->getOverlayElement("LeftCardText")->setCaption(figurinesPoints);
}

void Player::IsOnMovementSight()
{
    //RaySceneQueryResult& result = mRayScnQuery->execute();
    //RaySceneQueryResult::iterator hitResult = result.begin();
    //
    //Vector3 rayOrigin = mouseRay.getOrigin();
    //Vector3 rayDirection = mouseRay.getDirection();
    //
    //mNewPosition.x = rayOrigin.x + hitResult->distance * rayDirection.x;
    //mNewPosition.y = rayOrigin.y + hitResult->distance * rayDirection.y;
    //mNewPosition.z = rayOrigin.z + hitResult->distance * rayDirection.z;
    //
    //m_TargetPosition = mNewPosition;
    //m_TargetPosition.y = 2.f;
    //mCurrentSelectedPosition = mCurrentSelected->GetPosition();
    //mCurrentSelectedPosition.y = 2.f;
    //
    //mDistanceFromSelected = (mCurrentSelectedPosition - m_TargetPosition).length();
    //
    //if (mDistanceFromSelected > mCurrentSelected->GetMovementAction())
    //{
    //    mOnSightFromSelected = false;
    //    return;
    //}
    //
    //mOnSightFromSelected = true;
}

void Player::SelectFigurine(Figurines *figurineP)
{
    mCurrentSelected = figurineP;
    mCurrentSelected->OnSelected(true);
    mIsActorSelected = true;

    ShowFigurineCard(figurineP, false);
}

void Player::UnselectFigurine()
{
    // Unselect the last selected actor
    mCurrentSelected->OnSelected(false);
    mCurrentSelected = nullptr;
    mIsActorSelected = false;

    HideCards();
}

void Player::HideCards()
{
    m_OverlayManager->getOverlayElement("LeftCard")->hide();
    m_OverlayManager->getOverlayElement("RightCard")->hide();
}

bool Player::mouseMoved(const MouseMotionEvent& evt)
{
    if(mGameEngine.isGameLoaded)
        MouseRayTo3D(evt.x, evt.y);

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
