#include "GameEngine.h"
#include "InputsManager.h"

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

GameEngine::GameEngine()
{
}

void GameEngine::setup()
{
    // do not forget to call the base first
    ApplicationContext::setup();
    m_InputsManager = new  InputsManager(*this);
    addInputListener(m_InputsManager);

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
}

void GameEngine::QuitGame()
{
    getRoot()->queueEndRendering();
}

Actors* GameEngine::GetActor(const SceneNode* sceneNodeP)
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
