#include "CameraComponent.h"
#include <iostream>

#include "Figurines.h"
#include "QueryFlags.h"

CameraComponent::CameraComponent(GameEngine& gameEngineP) :
    m_GameEngine(gameEngineP),
    m_SceneManager(*gameEngineP.GetSceneManager()),
    m_RayScnQuery(0)
{
    m_RayScnQuery = m_SceneManager.createRayQuery(Ogre::Ray());
    m_RayScnQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);

    // ===== CAMERA ==== 
    /* Create the Camera */
    m_Camera = m_SceneManager.createCamera("mainCamera");
    m_Camera->setNearClipDistance(5); // specific to this sample
    m_Camera->setAutoAspectRatio(true); 

    /* Create a scene node for the camera */
    m_CamNode = m_SceneManager.getRootSceneNode()->createChildSceneNode();
    m_CamNode->attachObject(m_Camera);
    m_CamNode->setPosition(0, 100, 322);
    m_CamNode->pitch(Degree(-45));

    gameEngineP.addInputListener(this);
}

CameraComponent::~CameraComponent()
{
    m_SceneManager.destroyQuery(m_RayScnQuery);
}

void CameraComponent::Update(float deltaTime)
{
    UpdateCameraPosition(deltaTime);
}

bool CameraComponent::mouseWheelRolled(const MouseWheelEvent& evt)
{
    m_MouseWheelY = -evt.y;

    if (evt.y != 0)
    {
        return true;
    }
    else
        return false;
}

bool CameraComponent::mousePressed(const MouseButtonEvent& evt)
{
    Keycode key = evt.button;

    if (key == 1)
    {
        OnLBMouseDown(evt.x, evt.y);

        m_IsLMBDown = true;
    }

    if (key == 3)
    {
        m_IsRMBDown = true;
    }

    return false;
}

bool CameraComponent::mouseReleased(const MouseButtonEvent& evt)
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

bool CameraComponent::keyPressed(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    if (key == 'w')
    {
        m_MoveZ = -m_CameraSpeed;
    }

    if (key == 's')
    {
        m_MoveZ = m_CameraSpeed;
    }

    if (key == 'd')
    {
        m_MoveX = m_CameraSpeed;
    }

    if (key == 'a')
    {
        m_MoveX = -m_CameraSpeed;
    }

    return true;
}

bool CameraComponent::keyReleased(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    if (key == 'w')
    {
        m_MoveZ = 0;
    }

    if (key == 's')
    {
        m_MoveZ = 0;
    }

    if (key == 'd')
    {
        m_MoveX = 0;
    }

    if (key == 'a')
    {
        m_MoveX = 0;
    }

    return true;
}

void CameraComponent::frameRendered(const FrameEvent& evt)
{
        m_MouseWheelY = 0;
}

void CameraComponent::OnLBMouseDown(int mouseX, int mouseY)
{
    float width = mouseX / (float)m_GameEngine.getRenderWindow()->getWidth();
    float height = mouseY / (float)m_GameEngine.getRenderWindow()->getHeight();

    Ray mouseRay = m_Camera->getCameraToViewportRay( width, height);

    m_RayScnQuery->setRay(mouseRay);
    m_RayScnQuery->setSortByDistance(true);
    m_RayScnQuery->setQueryMask(QueryFlags::FIGURINE_MASK);

    RaySceneQueryResult& result = m_RayScnQuery->execute();
    RaySceneQueryResult::iterator it = result.begin();
    
    for (; it != result.end(); it++)
    {
        /* Check if Entity is has figurine flag */
        if (!it->movable || it->movable->getQueryFlags() != QueryFlags::FIGURINE_MASK)
        {
            if (m_IsActorSelected)
                UnselectFigurine();

            break;
        }

        /* Try to get entity's actor class and cast it to figurines */
        SceneNode* sceneNodeHit = it->movable->getParentSceneNode();
        Actors* getActor = m_GameEngine.GetActor(sceneNodeHit);
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

void CameraComponent::UnselectFigurine()
{
    // Unselect the last selected actor
    m_CurrentSelected->OnSelected(false);
    m_CurrentSelected = nullptr;
    m_IsActorSelected = false;
}

void CameraComponent::SelectFigurine(Figurines* figurineP)
{
    m_CurrentSelected = figurineP;
    m_CurrentSelected->OnSelected(true);
    m_IsActorSelected = true;
}

void CameraComponent::UpdateCameraPosition(float deltaTime)
{
    Vector3 newTranslation = m_CamNode->getPosition() + Vector3(m_MoveX * deltaTime, 0, m_MoveZ * deltaTime);

    if (abs(newTranslation.x) < m_ClampMaxX 
        && newTranslation.z > m_ClampMinZ && newTranslation.z < m_ClampMaxZ)
    {
        // Apply the accumulated translation
        m_CamNode->translate(m_MoveX * deltaTime, 0, m_MoveZ * deltaTime);
    }

    Zoom(deltaTime);
}

void CameraComponent::Zoom(float deltaTime)
{
    // Calculate the camera's forward vector based on its rotation
    Quaternion cameraOrientation = m_CamNode->getOrientation();
    Vector3 cameraDirection = cameraOrientation * Ogre::Vector3::UNIT_Z * (m_MouseWheelY * deltaTime * 500);

    Vector3 newTranslation = m_CamNode->getPosition() + cameraDirection;
    if (newTranslation.y < m_ClampMaxY && newTranslation.y > m_ClampMinY)
    {
        if (abs(newTranslation.x) < m_ClampMaxX
            && newTranslation.z > m_ClampMinZ && newTranslation.z < m_ClampMaxZ)
        {
            m_CamNode->translate(cameraDirection);
        }
    }
}
