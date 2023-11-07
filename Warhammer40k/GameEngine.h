#pragma once
#include "OgreApplicationContext.h"

#include "Ogre.h"

using namespace Ogre;
using namespace OgreBites;

class Actors;
class InputsManager;
class Ogre::SceneManager;

class GameEngine : public ApplicationContext
{
public:
    GameEngine();
    virtual ~GameEngine() {}

    virtual void setup() override;                                          // OgreApplicationContextBase
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe) override; // OgreApplicationContextBase

    /* Update Game Logic */
    void Update(float deltaTimeP);
    void QuitGame();

    void AddActor(Actors* actorP) { m_Actors.emplace_back(actorP) ; }
    InputsManager* GetInputsManager() { return m_InputsManager; }
    Ogre::SceneManager* GetSceneManager() { return m_SceneManager; }

private:
    InputsManager* m_InputsManager;
    Ogre::SceneManager* m_SceneManager;

    std::set<Keycode> keysPressed;
    std::vector<Actors*> m_Actors;

    bool ProcessUnbufferedInput(const Ogre::FrameEvent& fe);
};

