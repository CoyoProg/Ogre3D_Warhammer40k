#include "CameraComponent.h"
#include "InputsManager.h"
#include <iostream>

CameraComponent::CameraComponent(SceneManager& sceneManagerP, const InputsManager& inputsManagerP) :
    inputManager(inputsManagerP)
{
    // ===== CAMERA ==== 
    /* Create the Camera */
    m_Camera = sceneManagerP.createCamera("mainCamera");
    m_Camera->setNearClipDistance(5); // specific to this sample
    m_Camera->setAutoAspectRatio(true);

    /* Create a scene node for the camera */
    m_CamNode = sceneManagerP.getRootSceneNode()->createChildSceneNode();
    m_CamNode->attachObject(m_Camera);
    m_CamNode->setPosition(0, 150, 222);
    m_CamNode->pitch(Degree(-45));
}

void CameraComponent::Update(float deltaTime)
{
    updateCameraPosition(deltaTime);
}

void CameraComponent::updateCameraPosition(float deltaTime)
{
    float speed = 50.f;
    float moveX = 0.f;
    float moveZ = 0.f;

    for (const Keycode key : inputManager.getKeyPressed())
    {
        if (key == 'w')
        {
            moveZ -= speed;
        }

        if (key == 's')
        {
            moveZ += speed;
        }

        if (key == 'd')
        {
            moveX += speed;
        }

        if (key == 'a')
        {
            moveX -= speed;
        }
    }

    // Apply the accumulated translation
    m_CamNode->translate(moveX * deltaTime, 0, moveZ * deltaTime);
}
