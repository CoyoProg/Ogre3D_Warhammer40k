#pragma once
#include "OgreApplicationContext.h"
#include "Ogre.h"

#include "IListenerInputs.h"
using namespace Ogre;
using namespace OgreBites;

class Actors;
class InputsManager;

class GameEngine : public ApplicationContext, public IListenerInputs
{
public:
    GameEngine();
    virtual ~GameEngine() {}

    virtual void setup() override;                                           // OgreApplicationContextBase
    virtual bool frameRenderingQueued(const FrameEvent& fe) override;        // OgreApplicationContextBase

    /* Update Game Logic */
    void Update(float deltaTimeP);
    void QuitGame();

    void AddActor(Actors* actorP) { m_Actors.emplace_back(actorP) ; }
    Actors* GetActor(const SceneNode* sceneNodeP);

    InputsManager* GetInputsManager() { return m_InputsManager; }
    SceneManager* GetSceneManager() { return m_SceneManager; }

private:
    InputsManager* m_InputsManager;
    SceneManager* m_SceneManager;

    std::set<Keycode> m_KeysPressed;
    std::vector<Actors*> m_Actors;

    bool ProcessUnbufferedInput(const FrameEvent& fe);
};

