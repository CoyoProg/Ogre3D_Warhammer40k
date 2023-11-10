#pragma once
#include "OgreApplicationContext.h"
#include "Ogre.h"

using namespace Ogre;
using namespace OgreBites;

class Actors;
class Grid;

class GameEngine : public ApplicationContext, public InputListener
{
public:
    GameEngine();
    virtual ~GameEngine() {}

    virtual void setup() override;                                           // OgreApplicationContextBase
    virtual bool frameRenderingQueued(const FrameEvent& fe) override;        // OgreApplicationContextBase

    /* Update Game Logic */
    void Update(float deltaTimeP);

    void AddActor(Actors* actorP) { m_Actors.emplace_back(actorP) ; }
    void SetGrid(Grid* gridP) { m_grid = gridP; }

    Actors* GetActor(const SceneNode* sceneNodeP);
    Grid& GetGrid() { return *m_grid; }
    SceneManager* GetSceneManager() { return m_SceneManager; }

private:
    SceneManager* m_SceneManager;
    Grid* m_grid;

    std::set<Keycode> m_KeysPressed;
    std::vector<Actors*> m_Actors;

    /* Inputs */
    bool ProcessUnbufferedInput(const FrameEvent& fe);
    virtual bool keyPressed(const KeyboardEvent& evt) override;
};

