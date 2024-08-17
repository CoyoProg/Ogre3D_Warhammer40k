#pragma once
#include "OgreApplicationContext.h"
#include "Ogre.h"

using namespace Ogre;
using namespace OgreBites;

class Actors;
class Grid;
class TableTop;
class Player;
class OgreText;

class GameEngine : public ApplicationContext, public InputListener
{
public:
    GameEngine();
    virtual ~GameEngine() {};

    virtual void setup() override;                                           // OgreApplicationContextBase
    virtual bool frameRenderingQueued(const FrameEvent& fe) override;        // OgreApplicationContextBase

    /* Update Game Logic */
    void Update(float deltaTimeP);

    /* Actors */
    void AddActor(Actors* actorP) { m_Actors.emplace_back(actorP) ; }
    void RemoveActor(Actors* actorP);
    std::vector<Actors*> GetActors() { return m_Actors; }

    /* Player */
    void SetPlayer(Player* playerP) { m_Player = playerP; }
    void EndTurn();

    /* Grid */
    void SetGrid(Grid* gridP) { m_grid = gridP; }
    Grid& GetGrid() { return *m_grid; }

    /* Tabletop*/
    TableTop* tabletop;

    /* Managers */
    SceneManager* GetSceneManager() { return m_SceneManager; }
    Actors* GetSceneActor(const SceneNode* sceneNodeP);
    bool isGameLoaded{ false };

private:
    SceneManager* m_SceneManager;
    SceneNode* centerOfWorldNode;
    Grid* m_grid;
    OgreText* overlayWidgets;

    std::set<Keycode> m_KeysPressed;
    std::vector<Actors*> m_Actors;
    Player* m_Player;

    /* Inputs */
    bool ProcessUnbufferedInput(const FrameEvent& fe);
    virtual bool keyPressed(const KeyboardEvent& evt) override;


    const float TimeBetweenFlips = 5;
    float LastFlipTimer = 0;
    void flipTableTop(float deltaTime);
    bool isFlipping = false;
    bool changeDirection = false;
    int flipFlop = 1;
    float bumpHeight = 10.0f; // Height of the bump in units
    float flipProgress = 0.0f; // Progress of the flip animation
    float animationSpeed = 0.f;
    float flipSpeed = 180.0f; // Degrees per second
    float rotationAngle = 0;
};

