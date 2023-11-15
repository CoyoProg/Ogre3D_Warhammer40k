#include "Player.h"
#include "CameraComponent.h"
#include "Figurines.h"

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
}

void Player::OnLBMouseDown(int mouseX, int mouseY)
{
    MouseRayTo3D(mouseX, mouseY);

    RaySceneQueryResult& result = m_RayScnQuery->execute();
    RaySceneQueryResult::iterator it = result.begin();

    for (; it != result.end(); it++)
    {
        /* Check if Entity has figurine flag */
        if (!it->movable || it->movable->getQueryFlags() != QueryFlags::FIGURINE_MASK)
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

Ray Player::MouseRayTo3D(int mouseX, int mouseY)
{
    float width = mouseX / (float)m_GameEngine.getRenderWindow()->getWidth();
    float height = mouseY / (float)m_GameEngine.getRenderWindow()->getHeight();

    CameraComponent* camComp = (CameraComponent*)myComponents[0];
    Camera* m_Camera = camComp->getCamera();

    Ray mouseRay = m_Camera->getCameraToViewportRay(width, height);

    m_RayScnQuery->setRay(mouseRay);
    m_RayScnQuery->setSortByDistance(true);
    m_RayScnQuery->setQueryMask(QueryFlags::FIGURINE_MASK);

    return mouseRay;
}

void Player::OnRBMouseDown(int mouseX, int mouseY)
{
    if (m_CurrentSelected)
    {    
        Ray mouseRay = MouseRayTo3D(mouseX, mouseY);

        RaySceneQueryResult& result = m_RayScnQuery->execute();
        RaySceneQueryResult::iterator it = result.begin();

        for (; it != result.end(); it++)
        {
            /* Check if Entity has figurine flag */
            if (!it->movable || it->movable->getQueryFlags() != QueryFlags::FIGURINE_MASK)
            {
                Vector3 rayOrigin = mouseRay.getOrigin();
                Vector3 rayDirection = mouseRay.getDirection();

                Vector3 newPos;
                newPos.x = rayOrigin.x + it->distance * rayDirection.x;
                newPos.y = rayOrigin.y + it->distance * rayDirection.y;
                newPos.z = rayOrigin.z + it->distance * rayDirection.z;

                m_CurrentSelected->SetPosition(newPos);

                Vector3 targetPosition = it->movable->getParentNode()->getPosition();
                targetPosition.y = 2.f;
                Vector3 currentPosition = m_CurrentSelected->GetSceneNode()->getPosition();
                currentPosition.y = 2.f;

                Ray rayCast(currentPosition, targetPosition - currentPosition);
                m_RayScnQuery->setRay(rayCast);
                m_RayScnQuery->setSortByDistance(true);

                RaySceneQueryResult& newResult = m_RayScnQuery->execute();
                RaySceneQueryResult::iterator newIt = result.begin();

                for (; newIt != result.end(); newIt++)
                {
                    if (!newIt->movable || newIt->movable->getQueryFlags() != QueryFlags::FIGURINE_MASK)
                    {
                        std::cout << "Obstacle\n";
                        std::cout << newIt->movable->getName() << std::endl;
                        break;
                    }
                }

                break;
            }

            break;
        }
    }
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
