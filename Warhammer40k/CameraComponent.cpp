#include "CameraComponent.h"

#include <iostream>

CameraComponent::CameraComponent(GameManager &gameManagerP)
{
    // ===== CAMERA ==== 
    /* Create the Camera */
    mCamera = gameManagerP.GetSceneManager().createCamera("mainCamera");
    mCamera->setNearClipDistance(5); // specific to this sample
    mCamera->setAutoAspectRatio(true); 

    /* Create a scene node for the camera */
    mCamNode = gameManagerP.GetSceneManager().getRootSceneNode()->createChildSceneNode();
    mCamNode->attachObject(mCamera);
	mCamNode->setPosition(0, 100, 222);
	mCamNode->pitch(Degree(-45));

    gameManagerP.addInputListener(this);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTimeP)
{
    UpdateCameraPosition(deltaTimeP);
    mMouseWheelY = 0;
}

bool CameraComponent::mouseWheelRolled(const MouseWheelEvent& evt)
{
    mMouseWheelY = -evt.y;

    return evt.y != 0;
}

bool CameraComponent::keyPressed(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    switch (key)
    {
    case 'w':
    {
        mMoveZ = -mCameraSpeed;
        break;
    }
    case 's':
    {
        mMoveZ = mCameraSpeed;
        break;
    }
    case 'd':
    {
        mMoveX = mCameraSpeed;
        break;
    }
    case 'a':
    {
        mMoveX = -mCameraSpeed;
        break;
    }
    }

    return true;
}

bool CameraComponent::keyReleased(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    switch (key)
    {
    case 'w': 
    case 's':
    {
        mMoveZ = 0;
        break;
    }
    case 'd': 
    case 'a':
    {
        mMoveX = 0;
        break;
    }
    }

    return true;
}

void CameraComponent::UpdateCameraPosition(float deltaTimeP)
{
    Vector3 newTranslation = mCamNode->getPosition() + Vector3(mMoveX * deltaTimeP, 0, mMoveZ * deltaTimeP);

    if (abs(newTranslation.x) < mClampMaxX 
        && newTranslation.z > mClampMinZ 
        && newTranslation.z < mClampMaxZ)
    {
        // Apply the accumulated translation
        mCamNode->translate(mMoveX * deltaTimeP, 0, mMoveZ * deltaTimeP);
    }

    Zoom(deltaTimeP);
}

void CameraComponent::Zoom(float deltaTimeP)
{
    // Calculate the camera's forward vector based on its rotation
    Quaternion cameraOrientation = mCamNode->getOrientation();
    Vector3 cameraDirection = cameraOrientation * Ogre::Vector3::UNIT_Z * (mMouseWheelY * deltaTimeP * 500);

    Vector3 newTranslation = mCamNode->getPosition() + cameraDirection;
    if (newTranslation.y < mClampMaxY 
        && newTranslation.y > mClampMinY)
    {
        if (abs(newTranslation.x) < mClampMaxX
            && newTranslation.z > mClampMinZ 
            && newTranslation.z < mClampMaxZ)
        {
            mCamNode->translate(cameraDirection);
        }
    }
}
