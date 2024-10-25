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
	void InitializeGame();
	void InitializeRenderer();
	virtual bool frameRenderingQueued(const FrameEvent &fe) override;        // OgreApplicationContextBase

	/* Update Game Logic */
	void Update(float deltaTimeP);

	/* Actors */
	void AddActor(Actors *actorP) { mActors.emplace_back(actorP); }
	void RemoveActor(Actors *actorP);
	std::vector<Actors*> GetActors() { return mActors; }

	/* Player */
	void SetPlayer(Player *playerP) { mPlayer = playerP; }
	void EndTurn();

	/* Grid */
	void SetGrid(Grid *gridP) { mGrid = gridP; }
	Grid& GetGrid() { return *mGrid; }

	/* Tabletop*/
	TableTop* tabletop = nullptr;

	/* Managers */
	SceneManager* GetSceneManager() { return mSceneManager; }
	Actors* GetSceneActor(const SceneNode* sceneNodeP);
	bool isGameLoaded = false;

private:
	RenderWindow *mRenderWindow;
	SceneManager *mSceneManager = nullptr;
	SceneNode *mCenterOfWorldNode = nullptr;
	Grid *mGrid = nullptr;
	OgreText *mOverlayWidgets = nullptr;

	std::set<Keycode> mKeysPressed;
	std::vector<Actors*> mActors;
	Player *mPlayer = nullptr;

	/* Inputs */
	bool ProcessUnbufferedInput(const FrameEvent& fe);
	virtual bool keyPressed(const KeyboardEvent& evt) override;

	void FlipTableTop(float deltaTimeP);

	const float mTimeBetweenFlips = 5;
	float mLastFlipTimer = 0;
	bool mIsFlipping = false;
	bool mChangeDirection = false;
	int mFlipFlop = 1;
	float mBumpHeight = 10.0f; // Height of the bump in units
	float mFlipProgress = 0.0f; // Progress of the flip animation
	float mAnimationSpeed = 0.f;
	float mFlipSpeed = 180.0f; // Degrees per second
	float mRotationAngle = 0;
};

