#include "GameEngine.h"

/* Ogre Related */
#include "OgreRTShaderSystem.h"

/* C++ Related */
#include <iostream>

/* My scripts */
#include "Actors.h"
#include "GameLevel.h"
#include "Player.h"
#include "TableTop.h"

using namespace Ogre;

GameEngine::GameEngine() :
    tabletop(nullptr),
    m_SceneManager(nullptr),
    m_grid(nullptr)
{
}

void GameEngine::setup()
{
    // do not forget to call the base first
    ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    Root* root = getRoot();
    m_SceneManager = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(m_SceneManager);

    /* Set ambient light for the Scene Manager */
    m_SceneManager->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    /* Load mesh into ressources */
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..\\Assets/Materials", "FileSystem", "AssetsGroup");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..\\Assets/Meshes", "FileSystem", "AssetsGroup");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    /*Load Level */
    GameLevel::LoadLevel(*this);
}

void GameEngine::Update(float deltaTimeP)
{
    for (auto& actors : m_Actors)
    {
        actors->Update(deltaTimeP);
    }

    debugDelay -= deltaTimeP;


    if (debugDelay <= 0)
    {
        debugDelay = 2;
        isFlipping = true;
    }

    flipTableTop(deltaTimeP);
}

Actors* GameEngine::GetSceneActor(const SceneNode* sceneNodeP)
{
    for (auto actors : m_Actors)
    {
        if (actors->GetSceneNode() == sceneNodeP)
        {
            return actors;
        }
    }

    return nullptr;
}

bool GameEngine::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    // Calculate the delta time (time since the last frame)
    float deltaTime = fe.timeSinceLastFrame;

    // Handle game logic and update here
    Update(deltaTime);

    bool ret = ApplicationContext::frameRenderingQueued(fe);

    if (!ProcessUnbufferedInput(fe))
        return false;

    return ret;
}

bool GameEngine::ProcessUnbufferedInput(const Ogre::FrameEvent& fe)
{
	return true;
}

bool GameEngine::keyPressed(const KeyboardEvent& evt)
{
    Keycode key = evt.keysym.sym;

    if (key == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }

    return true;
}

void GameEngine::flipTableTop(float deltaTime)
{
    if (isFlipping)
    {
        //flipProgress += deltaTime * animationSpeed;
        //
        //float rotationAngle = flipSpeed * deltaTime;
        //flipProgress += rotationAngle;
        //tabletop->GetSceneNode()->yaw(Ogre::Degree(rotationAngle));
        //
        //float translationAmount = bumpHeight * deltaTime;
        //tabletop->GetSceneNode()->translate(Ogre::Vector3::UNIT_Y * translationAmount, Ogre::Node::TS_LOCAL);
        //
        //// Check if the flip is complete
        //if (flipProgress >= 180.0f)
        //{
        //    isFlipping = false;
        //    flipFlop = -flipFlop;
        //    flipProgress = 0.0f; // Reset the flip progress for future flips
        //
        //    Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
        //    orientation.FromAngleAxis(Degree(90 * flipFlop), Ogre::Vector3::UNIT_Y);
        //    tabletop->GetSceneNode()->setOrientation(orientation);
        //
        //    Vector3 finalPos = tabletop->GetSceneNode()->getPosition();
        //    tabletop->GetSceneNode()->setPosition(Vector3(finalPos.x, 0, finalPos.z));
        //}
    }
}
