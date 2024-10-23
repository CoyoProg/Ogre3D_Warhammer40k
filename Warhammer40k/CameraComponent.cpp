#include "CameraComponent.h"

#include <iostream>

CameraComponent::CameraComponent(GameEngine &gameEngineP)
{
    // ===== CAMERA ==== 
    /* Create the Camera */
    mCamera = gameEngineP.GetSceneManager()->createCamera("mainCamera");
    mCamera->setNearClipDistance(5); // specific to this sample
    mCamera->setAutoAspectRatio(true); 


    /* Create a scene node for the camera */
    mCamNode = gameEngineP.GetSceneManager()->getRootSceneNode()->createChildSceneNode();
    mCamNode->attachObject(mCamera);
	mCamNode->setPosition(0, 100, 222);
	mCamNode->pitch(Degree(-45));

    gameEngineP.addInputListener(this);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTimeP)
{
    UpdateCameraPosition(deltaTimeP);
}

bool CameraComponent::mouseWheelRolled(const MouseWheelEvent& evt)
{
    mMouseWheelY = -evt.y;

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
        mMoveZ = -mCameraSpeed;
    }

    if (key == 's')
    {
        mMoveZ = mCameraSpeed;
    }

    if (key == 'd')
    {
        mMoveX = mCameraSpeed;
    }

    if (key == 'a')
    {
        mMoveX = -mCameraSpeed;
    }

    return true;
}

bool CameraComponent::keyReleased(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    if (key == 'w')
    {
        mMoveZ = 0;
    }

    if (key == 's')
    {
        mMoveZ = 0;
    }

    if (key == 'd')
    {
        mMoveX = 0;
    }

    if (key == 'a')
    {
        mMoveX = 0;
    }

    return true;
}

void CameraComponent::frameRendered(const FrameEvent& evt)
{
        mMouseWheelY = 0;
}

void CameraComponent::UpdateCameraPosition(float deltaTimeP)
{
    Vector3 newTranslation = mCamNode->getPosition() + Vector3(mMoveX * deltaTimeP, 0, mMoveZ * deltaTimeP);

    if (abs(newTranslation.x) < mClampMaxX 
        && newTranslation.z > mClampMinZ && newTranslation.z < mClampMaxZ)
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
    if (newTranslation.y < mClampMaxY && newTranslation.y > mClampMinY)
    {
        if (abs(newTranslation.x) < mClampMaxX
            && newTranslation.z > mClampMinZ && newTranslation.z < mClampMaxZ)
        {
            mCamNode->translate(cameraDirection);
        }
    }
}
