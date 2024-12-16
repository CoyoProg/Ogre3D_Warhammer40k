#pragma once
#include "Ogre.h"
#include "OgreInput.h"

using namespace Ogre;
using namespace OgreBites;

class Actors;
class Grid;
class TableTop;
class Player;
class OgreText;

class GameEngine;

class GameManager : public InputListener 
{
public:
	/* Update the game logic */
	void Update(float deltaTimeP);

	void SetupGameManager(RenderWindow& renderWindowP, SceneManager& sceneManagerP, GameEngine& engineP);
	void LoadResources();
	void InitializeGame();

	void AddInputListener(InputListener* listenerP);

	void AddActor(Actors *actorP) { mActors.emplace_back(actorP); }
	void RemoveActor(Actors *actorP);

	std::vector<Actors*> GetActors() { return mActors; }
	Grid& GetGrid() { return *mGrid; }
	TableTop& GetTableTop() { return *mTableTop; }

	void EndTurn();
	void OnNewTurn();

	SceneManager& GetSceneManager() { return *mSceneManager; }
	Actors* GetSceneActor(const SceneNode& sceneNodeP);
	RenderWindow& GetRenderWindow() { return *mRenderWindow; }

	bool isGameLoaded = false;

private:
	virtual bool keyPressed(const KeyboardEvent& evt) override;

	RenderWindow *mRenderWindow = nullptr;
	SceneManager *mSceneManager = nullptr;
	GameEngine* mGameEngine = nullptr;

	Grid *mGrid = nullptr;
	OgreText *mOverlayWidgets = nullptr;
	TableTop* mTableTop = nullptr;
	Player *mPlayer = nullptr;

	std::set<Keycode> mKeysPressed;
	std::vector<Actors*> mActors;

	const float EndTurnCooldown = 5.0f;
	float EndTurnTimer = 0.0f;
};

