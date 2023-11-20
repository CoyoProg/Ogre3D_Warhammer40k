#include "Player.h"
#include "CameraComponent.h"
#include "PathFindingComponent.h"
#include "Figurines.h"
#include "Grid.h"

#include <OgreSceneQuery.h>
#include "QueryFlags.h"
#include <iostream>

Player::Player(GameEngine& gameEngineP) :
	m_GameEngine (gameEngineP),
    m_RayScnQuery(0)
{
	CameraComponent* camera = new CameraComponent(gameEngineP);
	gameEngineP.getRenderWindow()->addViewport(camera->getCamera());
	AddComponent(camera);

    pathfinding = new PathFindingComponent(gameEngineP);
    AddComponent(pathfinding);

    m_RayScnQuery = gameEngineP.GetSceneManager()->createRayQuery(Ogre::Ray());
    m_RayScnQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

    gameEngineP.addInputListener(this);
}

Player::~Player()
{
    m_GameEngine.GetSceneManager()->destroyQuery(m_RayScnQuery);
}

void Player::Update(float deltaTime)
{
	Actors::Update(deltaTime);

    /* We dont want it to be called every framed */
    functionDelay -= deltaTime;
    if (m_CurrentSelected && functionDelay <= 0)
    {
        functionDelay = .2f;
        IsTargetOnSight();
    }
}

Ray Player::MouseRayTo3D(int mouseX, int mouseY)
{
    float width = mouseX / (float)m_GameEngine.getRenderWindow()->getWidth();
    float height = mouseY / (float)m_GameEngine.getRenderWindow()->getHeight();

    CameraComponent* camComp = (CameraComponent*)myComponents[0];
    Camera* m_Camera = camComp->getCamera();

    mouseRay = m_Camera->getCameraToViewportRay(width, height);

    m_RayScnQuery->setRay(mouseRay);
    m_RayScnQuery->setSortByDistance(true);
    m_RayScnQuery->setQueryMask(FIGURINE_MASK | OBSTACLE_MASK);

    return mouseRay;
}

void Player::OnLBMouseDown(int mouseX, int mouseY)
{
    MouseRayTo3D(mouseX, mouseY);

    RaySceneQueryResult& result = m_RayScnQuery->execute();
    RaySceneQueryResult::iterator it = result.begin();

    for (; it != result.end(); it++)
    {
        /* Check if Entity has figurine flag */
        if (!it->movable || it->movable->getQueryFlags() != FIGURINE_MASK)
        {
            if (m_IsActorSelected)
                UnselectFigurine();

            break;
        }

        /* Try to get entity's actor class and cast it to figurines */
        SceneNode* sceneNodeHit = it->movable->getParentSceneNode();
        Actors* getActor = m_GameEngine.GetSceneActor(sceneNodeHit);
        if (!getActor)
            break;
        Figurines* temporary = dynamic_cast<Figurines*>(getActor);
        if (!temporary)
            break;

        if (!m_IsActorSelected)
        {
            SelectFigurine(temporary);
            break; // To select only one actor at a time
        }

        if (temporary->GetSceneNode() != m_CurrentSelected->GetSceneNode())
        {
            UnselectFigurine();
            SelectFigurine(temporary);
            break; // To select only one actor at a time
        }

        UnselectFigurine();
        break;
    }
}

void Player::OnRBMouseDown(int mouseX, int mouseY)
{
    /* Move or attack the target if a Figurine is currently selected */
    if (m_CurrentSelected)
    { 
        IsTargetOnSight();

        if (!m_OnSightFromSelected || !m_CurrentSelected->IsSleeping())
            return;

        RaySceneQueryResult& result = m_RayScnQuery->execute();
        RaySceneQueryResult::iterator hitResult = result.begin();

        for (; hitResult != result.end(); hitResult++)
        {
            /* We check if we click on a Figurine/Obstacle 
               And Move the Selected Figirune to the new Position if not */
            if (hitResult->movable->getQueryFlags() != FIGURINE_MASK && hitResult->movable->getQueryFlags() != OBSTACLE_MASK)
            {
                /* We cast a second Ray to check if there is an Obstacle
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
                        bool pathFound = pathfinding->FindPath(m_CurrentSelectedPosition, m_TargetPosition, m_CurrentSelected->GetMovementAction());
                        
                        if (pathFound)
                        {
                            std::vector<Vector3> finalPath = pathfinding->GetFinalPath();

                            finalPath.pop_back();
                            finalPath.emplace_back(m_TargetPosition);

                            m_CurrentSelected->MoveTo(finalPath);
                        }

                        return;
                    }
                }

                m_CurrentSelected->SetPosition(m_NewPosition);
                break;
            }

            if(hitResult->movable->getQueryFlags() == QueryFlags::FIGURINE_MASK)
            {
                if (m_CurrentSelected->GetEntity() == hitResult->movable)
                    return;

                std::cout << "ATTACK " << hitResult->movable->getName() << std::endl;
            }
            break;
        }
    }
}

void Player::IsTargetOnSight()
{
    RaySceneQueryResult& result = m_RayScnQuery->execute();
    RaySceneQueryResult::iterator hitResult = result.begin();

    Vector3 rayOrigin = mouseRay.getOrigin();
    Vector3 rayDirection = mouseRay.getDirection();

    m_NewPosition.x = rayOrigin.x + hitResult->distance * rayDirection.x;
    m_NewPosition.y = rayOrigin.y + hitResult->distance * rayDirection.y;
    m_NewPosition.z = rayOrigin.z + hitResult->distance * rayDirection.z;

    m_TargetPosition = m_NewPosition;
    m_TargetPosition.y = 2.f;
    m_CurrentSelectedPosition = m_CurrentSelected->GetPosition();
    m_CurrentSelectedPosition.y = 2.f;

    float distanceFromSelected = (m_CurrentSelectedPosition - m_TargetPosition).length();

    if (distanceFromSelected > m_CurrentSelected->GetMovementAction())
    {
        m_OnSightFromSelected = false;
        return;
    }

    m_OnSightFromSelected = true;
}

void Player::SelectFigurine(Figurines* figurineP)
{
    m_CurrentSelected = figurineP;
    m_CurrentSelected->OnSelected(true);
    m_IsActorSelected = true;
}

void Player::UnselectFigurine()
{
    // Unselect the last selected actor
    m_CurrentSelected->OnSelected(false);
    m_CurrentSelected = nullptr;
    m_IsActorSelected = false;
}

bool Player::mouseMoved(const MouseMotionEvent& evt)
{
    if (m_CurrentSelected)
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
