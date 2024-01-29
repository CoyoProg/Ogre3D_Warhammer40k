#include "CameraComponent.h"
#include <iostream>

CameraComponent::CameraComponent(GameEngine& gameEngineP)
{
    // ===== CAMERA ==== 
    /* Create the Camera */
    m_Camera = gameEngineP.GetSceneManager()->createCamera("mainCamera");
    m_Camera->setNearClipDistance(5); // specific to this sample
    m_Camera->setAutoAspectRatio(true); 


    /* Create a scene node for the camera */
    m_CamNode = gameEngineP.GetSceneManager()->getRootSceneNode()->createChildSceneNode();
    m_CamNode->attachObject(m_Camera);
	m_CamNode->setPosition(0, 100, 222);
	m_CamNode->pitch(Degree(-45));

    gameEngineP.addInputListener(this);
}

CameraComponent::~CameraComponent()
{
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
