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

struct FlipTableAnimation
{
	static constexpr float bumpHeight = 10.0f; // Height of the bump in units
	static constexpr float flipsCooldown = 5;

	float animationProgress = 0.0f; // Progress of the flip animation
	float animationSpeed = 0.f;
	float rotationAngle = 0;

	float flipTimer = 0;
	bool isFlipping = false;
	bool shouldChangeDir = false;
	int flipFlop = 1;
};

class GameManager : public ApplicationContext, public InputListener
{
public:
	GameManager();
	virtual ~GameManager() {};

	/* Update the game logic */
	void Update(float deltaTimeP);

	virtual void setup() override;                                           // OgreApplicationContextBase
	virtual bool frameRenderingQueued(const FrameEvent &fe) override;        // OgreApplicationContextBase
	void InitializeGame();
	void InitializeRenderer();

	void AddActor(Actors *actorP) { mActors.emplace_back(actorP); }
	void RemoveActor(Actors *actorP);
	std::vector<Actors*> GetActors() { return mActors; }

	void SetGrid(Grid* gridP) { mGrid = gridP; }
	Grid& GetGrid() { return *mGrid; }

	void SetPlayer(Player *playerP) { mPlayer = playerP; }
	void EndTurn();

	SceneManager* GetSceneManager() { return mSceneManager; }
	Actors* GetSceneActor(const SceneNode* sceneNodeP);

	TableTop* tabletop = nullptr;
	bool isGameLoaded = false;

private:
	bool ProcessUnbufferedInput(const FrameEvent& fe);
	virtual bool keyPressed(const KeyboardEvent& evt) override;

	void FlipTableTop(float deltaTimeP);

	RenderWindow *mRenderWindow;
	SceneManager *mSceneManager = nullptr;
	SceneNode *mCenterOfWorldNode = nullptr;
	Grid *mGrid = nullptr;
	OgreText *mOverlayWidgets = nullptr;
	Player* mPlayer = nullptr;

	std::set<Keycode> mKeysPressed;
	std::vector<Actors*> mActors;
	
	FlipTableAnimation mFlipAnimation;
};

