#pragma once
#include "OgreApplicationContext.h"
#include "Ogre.h"

using namespace Ogre;
using namespace OgreBites;

class Actors;
class Grid;
class TableTop;

class GameEngine : public ApplicationContext, public InputListener
{
public:
    GameEngine();
    virtual ~GameEngine() {};

    virtual void setup() override;                                           // OgreApplicationContextBase
    virtual bool frameRenderingQueued(const FrameEvent& fe) override;        // OgreApplicationContextBase

    /* Update Game Logic */
    void Update(float deltaTimeP);

    void AddActor(Actors* actorP) { m_Actors.emplace_back(actorP) ; }
    std::vector<Actors*> GetActors() { return m_Actors; }
    void SetGrid(Grid* gridP) { m_grid = gridP; }
    Grid& GetGrid() { return *m_grid; }
    TableTop* tabletop;

    SceneManager* GetSceneManager() { return m_SceneManager; }
    Actors* GetSceneActor(const SceneNode* sceneNodeP);
private:
    SceneManager* m_SceneManager;
    Grid* m_grid;

    std::set<Keycode> m_KeysPressed;
    std::vector<Actors*> m_Actors;

    /* Inputs */
    bool ProcessUnbufferedInput(const FrameEvent& fe);
    virtual bool keyPressed(const KeyboardEvent& evt) override;


    float debugDelay = 2;
    void flipTableTop(float deltaTime);
    bool isFlipping = false;
    int flipFlop = 1;
    float bumpHeight = 5.0f; // Height of the bump in units
    float flipProgress = 0.0f; // Progress of the flip animation
    float animationSpeed = 1;
    float flipSpeed = 180.0f; // Degrees per second

};

