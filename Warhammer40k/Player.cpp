#include "Player.h"
#include "CameraComponent.h"

#include "Figurines.h"
#include "Grid.h"
#include "OgreText.h"

#include <OgreSceneQuery.h>
#include "QueryFlags.h"
#include <iostream>

Player::Player(GameEngine& gameEngineP) :
	m_GameEngine (gameEngineP),
    m_RayScnQuery(0)
{
    /*  Add Components */
	CameraComponent* camera = new CameraComponent(gameEngineP);
	gameEngineP.getRenderWindow()->addViewport(camera->getCamera());
	AddComponent(camera);

    /* Create the Ray */
    m_RayScnQuery = gameEngineP.GetSceneManager()->createRayQuery(Ogre::Ray());
    m_RayScnQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

    /* Add listener */
    gameEngineP.addInputListener(this);
    gameEngineP.SetPlayer(this);

    /* Get Overlay References */
    m_OverlayManager = OverlayManager::getSingletonPtr();

    /* Create the Dice */
    //m_Dice = m_GameEngine.GetSceneManager()->createEntity("Dice", "Dice.mesh");
    //m_Dice->setMaterialName("Dice");
    //SceneNode* diceNode = m_GameEngine.GetSceneManager()->getRootSceneNode()->createChildSceneNode("DiceNode");
    //diceNode->attachObject(m_Dice);
    //diceNode->setPosition(Vector3(0, 10, 0));
    //diceNode->setScale(Vector3(.05f, .05f, .05f));
}

Player::~Player()
{
    m_GameEngine.GetSceneManager()->destroyQuery(m_RayScnQuery);
}

void Player::Update(float deltaTime)
{
	Actors::Update(deltaTime);

    /* We dont want it to be called every framed */
    //functionDelay -= deltaTime;
    //if (m_CurrentSelected && functionDelay <= 0)
    //{
    //    functionDelay = .2f;
    //    IsOnMovementSight();
    //}
}

void Player::MouseRayTo3D(int mouseX, int mouseY)
{
    // Cast Ray //
    float width = mouseX / (float)m_GameEngine.getRenderWindow()->getWidth();
    float height = mouseY / (float)m_GameEngine.getRenderWindow()->getHeight();

    CameraComponent* camComp = (CameraComponent*)myComponents[0];
    Camera* m_Camera = camComp->getCamera();

    mouseRay = m_Camera->getCameraToViewportRay(width, height);

    m_RayScnQuery->setRay(mouseRay);
    m_RayScnQuery->setSortByDistance(true);
    m_RayScnQuery->setQueryMask(FIGURINE_MASK | OBSTACLE_MASK);


    // Check for Collisions //
    RaySceneQueryResult& result = m_RayScnQuery->execute();
    RaySceneQueryResult::iterator it = result.begin();
    m_CurrentMouseOver = nullptr;

    /* Check if the entity is not a figurine */
    if (!it->movable || it->movable->getQueryFlags() != FIGURINE_MASK)
        return;

    /* Try to get the entity's actor class and cast it to figurines */
    SceneNode* sceneNodeHit = it->movable->getParentSceneNode();
    Actors* getActor = m_GameEngine.GetSceneActor(sceneNodeHit);
    if (!getActor)
        return;

    Figurines* temporary = dynamic_cast<Figurines*>(getActor);
    if (!temporary)
        return;

    m_CurrentMouseOver = temporary;
}

void Player::OnLBMouseDown(int mouseX, int mouseY)
{
    /* Unselect current selection if nothing is Moused Over */
    if (!m_CurrentMouseOver && m_IsActorSelected)
    {
        UnselectFigurine();
        return;
    }

    if (!m_CurrentMouseOver)
    {
        HideCards();
        return;
    }

    /* Check if selecting an Enemy's figurine */
    if (m_CurrentMouseOver->getOwner() != PlayerID)
    {
        ShowFigurineCard(m_CurrentMouseOver, true);
        return;
    }

    /* If no current selection, select the new figurine */
    if (!m_IsActorSelected)
    {
        SelectFigurine(m_CurrentMouseOver);
        return;
    }

    /* Check if current selection isn't the same as the new selected figurine */
    if (m_CurrentMouseOver->GetSceneNode() != m_CurrentSelected->GetSceneNode())
    {
        UnselectFigurine();
        SelectFigurine(m_CurrentMouseOver);
    }else
        UnselectFigurine();
}

void Player::OnRBMouseDown(int mouseX, int mouseY)
{
    /* Check if a figurine is selected */
    if (m_CurrentSelected)
    { 
        /* Check if the figurine is already doing an action */
        if (!m_CurrentSelected->IsSleeping())
            return;

        /* Cast a Ray to check what we right click on */
        RaySceneQueryResult& result = m_RayScnQuery->execute();
        RaySceneQueryResult::iterator hitResult = result.begin();

        for (; hitResult != result.end(); hitResult++)
        {
            /* Check if the RayCast collide with a Figurine or an Obstacle */
            if (hitResult->movable->getQueryFlags() != FIGURINE_MASK && hitResult->movable->getQueryFlags() != OBSTACLE_MASK)
            {
                //IsOnMovementSight();

                //if (!m_OnSightFromSelected)
                //    return;

                /* Calculate the new Position to move to */
                m_CurrentSelectedPosition = m_CurrentSelected->GetPosition();
                m_CurrentSelectedPosition.y = 2.f;

                Vector3 rayOrigin = mouseRay.getOrigin();
                Vector3 rayDirection = mouseRay.getDirection();

                m_NewPosition.x = rayOrigin.x + hitResult->distance * rayDirection.x;
                m_NewPosition.y = rayOrigin.y + hitResult->distance * rayDirection.y;
                m_NewPosition.z = rayOrigin.z + hitResult->distance * rayDirection.z;

                m_TargetPosition = m_NewPosition;
                m_TargetPosition.y = 2.f;

                /* Cast a second Ray to check if there is an Obstacle
                   Between the Figurine and the targetPosition */
                Ray rayCast(m_CurrentSelectedPosition, m_TargetPosition - m_CurrentSelectedPosition);
                m_RayScnQuery->setRay(rayCast);
                m_RayScnQuery->setSortByDistance(true);
                m_RayScnQuery->execute();

                RaySceneQueryResult& secondResult = m_RayScnQuery->execute();
                RaySceneQueryResult::iterator secondHitResult = secondResult.begin();

                for (; secondHitResult != secondResult.end(); secondHitResult++)
                {
                    if (secondHitResult->distance > 1)
                        break;

                    if (secondHitResult->movable->getQueryFlags() == OBSTACLE_MASK)
                    {
                        /* Trigger Figurine MoveTo
                        The figurine will move around the obstacle */
                        m_CurrentSelected->MoveTo(m_TargetPosition);

                        /* Update Left Card Text */
                        float movementAction = m_CurrentSelected->GetMovementAction();
                        int healthPoint = m_CurrentSelected->GetHealthPoint();
                        SetCardTextValues(movementAction, healthPoint);

                        return;
                    }
                }

                /* Trigger move straight if there is no obstacles on the way */
                m_CurrentSelected->MoveStraight(m_NewPosition);

                /* Update Left Card Text */
                float movementAction = m_CurrentSelected->GetMovementAction();
                int healthPoint = m_CurrentSelected->GetHealthPoint();
                SetCardTextValues(movementAction, healthPoint);

                break;
            }

            /* Check if the selected figurine has any ActionPoint Left*/
            if (m_CurrentSelected->GetActionPoint() <= 0)
                return;

            if(hitResult->movable->getQueryFlags() == QueryFlags::FIGURINE_MASK)
            {
                /* If right clicking on the current selected figurine, return */
                if (m_CurrentSelected->GetEntity() == hitResult->movable)
                    return;


                /* Try to get entity's actor class and cast it to figurines */
                SceneNode* sceneNodeHit = hitResult->movable->getParentSceneNode();
                Actors* getActor = m_GameEngine.GetSceneActor(sceneNodeHit);
                if (!getActor)
                    return;

                Figurines* target = dynamic_cast<Figurines*>(getActor);
                if (!target)
                    return;

                /* Check if targeting an ally */
                if (target->getOwner() == m_CurrentSelected->getOwner())
                    return; // Heal or Buff Ally Here

                /* Check if the enemy is On Shooting Sight */
                if (distanceFromSelected > m_CurrentSelected->GetAttackRange())
                    return;

                m_CurrentSelected->Attack(target);
            }
            break;
        }
    }
}

void Player::ShowFigurineCard(Figurines* figurineP, bool isRightCard)
{
    /* Update the Right Card Texts*/
    float movementAction = figurineP->GetMovementAction();
    int healthPoint = figurineP->GetHealthPoint();
    SetCardTextValues(movementAction, healthPoint, true);

    std::string figurineName = figurineP->GetEntity()->getName();

    /* Show right card if it's an enemy card */
    if (isRightCard)
    {
        m_OverlayManager->getOverlayElement("RightCardNameText")->setCaption(figurineName);
        m_OverlayManager->getOverlayElement("RightCard")->show();

        return;
    }

    /* Show left card if it's a player card */
    m_OverlayManager->getOverlayElement("LeftCardNameText")->setCaption(figurineName);
    m_OverlayManager->getOverlayElement("LeftCard")->show();
}

void Player::SetCardTextValues(float movementPoint, int healthPoint, bool isRightCard)
{
    // Convert to string with precision of 1 decimal place
    std::string result = std::to_string(movementPoint);
    size_t dotPos = result.find('.');
    if (dotPos != std::string::npos && result.length() > dotPos + 2)
    {
        result = result.substr(0, dotPos + 2);
    }

    std::string figurinesPoints = std::string("Health Point: " + std::to_string(healthPoint) + " \n\n" + std::string("Movement Point: ") + result);
   
    if (isRightCard)
    {
        m_OverlayManager->getOverlayElement("RightCardText")->setCaption(figurinesPoints);
        return;
    }
    
    m_OverlayManager->getOverlayElement("LeftCardText")->setCaption(figurinesPoints);
}

void Player::IsOnMovementSight()
{
    //RaySceneQueryResult& result = m_RayScnQuery->execute();
    //RaySceneQueryResult::iterator hitResult = result.begin();
    //
    //Vector3 rayOrigin = mouseRay.getOrigin();
    //Vector3 rayDirection = mouseRay.getDirection();
    //
    //m_NewPosition.x = rayOrigin.x + hitResult->distance * rayDirection.x;
    //m_NewPosition.y = rayOrigin.y + hitResult->distance * rayDirection.y;
    //m_NewPosition.z = rayOrigin.z + hitResult->distance * rayDirection.z;
    //
    //m_TargetPosition = m_NewPosition;
    //m_TargetPosition.y = 2.f;
    //m_CurrentSelectedPosition = m_CurrentSelected->GetPosition();
    //m_CurrentSelectedPosition.y = 2.f;
    //
    //distanceFromSelected = (m_CurrentSelectedPosition - m_TargetPosition).length();
    //
    //if (distanceFromSelected > m_CurrentSelected->GetMovementAction())
    //{
    //    m_OnSightFromSelected = false;
    //    return;
    //}
    //
    //m_OnSightFromSelected = true;
}

void Player::SelectFigurine(Figurines* figurineP)
{
    m_CurrentSelected = figurineP;
    m_CurrentSelected->OnSelected(true);
    m_IsActorSelected = true;

    ShowFigurineCard(figurineP, false);
}

void Player::UnselectFigurine()
{
    // Unselect the last selected actor
    m_CurrentSelected->OnSelected(false);
    m_CurrentSelected = nullptr;
    m_IsActorSelected = false;

    HideCards();
}

void Player::HideCards()
{
    m_OverlayManager->getOverlayElement("LeftCard")->hide();
    m_OverlayManager->getOverlayElement("RightCard")->hide();
}

bool Player::mouseMoved(const MouseMotionEvent& evt)
{
    if(m_GameEngine.isGameLoaded)
        MouseRayTo3D(evt.x, evt.y);

    return false;
}

bool Player::mousePressed(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        OnLBMouseDown(evt.x, evt.y);
        m_IsLMBDown = true;
    }

    if (key == 3)
    {
        OnRBMouseDown(evt.x, evt.y);
        m_IsRMBDown = true;
    }

    return false;
}

bool Player::mouseReleased(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        m_IsLMBDown = false;
    }

    if (key == 3)
    {
        m_IsRMBDown = false;
    }

    return false;
}
