#include "GameManager.h"
#include "GameEngine.h"
#include "OgreRTShaderSystem.h"
#include "OgreText.h"
#include <OgreFontManager.h>
#include <OgreTrays.h>

#include "Actors.h"
#include "GameLevel.h"
#include "Player.h"
#include "TableTop.h"
#include "Grid.h"
#include "QueryFlags.h"
#include "Obstacles.h"

#include <iostream>

using namespace Ogre;

void GameManager::SetupGameManager(RenderWindow& renderWindowP, SceneManager& sceneManagerP, GameEngine& engineP)
{
	mRenderWindow = &renderWindowP;
	mSceneManager = &sceneManagerP;
	mGameEngine = &engineP;

	AddInputListener(this);
}

void GameManager::LoadResources()
{
	/* Load mesh, materials, and fonts into ressources */
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"..\\Assets/Materials",
		"FileSystem",
		"AssetsGroup"
	);

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"..\\Assets/Meshes",
		"FileSystem",
		"AssetsGroup"
	);

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
		"..\\Assets/Fonts",
		"FileSystem",
		"MyFontGroup"
	);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GameManager::AddInputListener(InputListener* listenerP)
{
	mGameEngine->addInputListener(listenerP);
}

void GameManager::InitializeGame()
{
	/* Add Player */
	OverlayManager* overlayManager = OverlayManager::getSingletonPtr();
	mPlayer = new Player(*this, *overlayManager);

	/* Create the Grid */
	mGrid = new Grid(*this);

	/* Create TableTop*/
	mTableTop = new TableTop(*this);
	AddActor(mTableTop);

	/* Load Level */
	GameLevel::LoadEnvironment(*mSceneManager, *this, *mGrid);
	GameLevel::SpawnActors(*this, *mGrid);

	/* Create Widgets */
	mOverlayWidgets = new OgreText(*mRenderWindow);
	mOverlayWidgets->GetPlayerTextElement()->setCaption("Player One");
}

void GameManager::Update(float deltaTimeP)
{
	for (auto& actors : mActors)
	{
		actors->Update(deltaTimeP);
	}

	if (mPlayer)
	{
		mPlayer->Update(deltaTimeP);
	}

	EndTurnTimer += deltaTimeP;
}

void GameManager::RemoveActor(Actors* actorP)
{
	/* Find the iterator pointing to the actor */
	auto iter = std::find(mActors.begin(), mActors.end(), actorP);

	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}

	delete actorP;
}

void GameManager::EndTurn()
{
	// NEEDS TO CHECK IF ALL THE ACTORS AREN'T MOVING
	if (EndTurnTimer < EndTurnCooldown) return;
	EndTurnTimer = 0.0f;

	mPlayer->OnEndTurn();

	for (auto actors : mActors)
	{
		actors->OnEndTurn();
	}

	int playerTurns = mPlayer->GetCurrentPlayer();

	if (playerTurns == 1)
	{
		mOverlayWidgets->GetPlayerTextElement()->setCaption("Player One");
		mOverlayWidgets->GetPlayerTextElement()->setColour(Ogre::ColourValue(0.0f, 0.f, 0.7f, 1.f));
	}
	else
	{
		mOverlayWidgets->GetPlayerTextElement()->setCaption("Player Two");
		mOverlayWidgets->GetPlayerTextElement()->setColour(Ogre::ColourValue(0.7f, 0.f, 0.0f, 1.f));
	}
}

void GameManager::OnNewTurn()
{
	mGrid->OnFlip();

	for (auto actors : mActors)
	{
		actors->OutEndTurn();

		if (!actors->GetObject()) continue;

		if (actors->GetObject()->getQueryFlags() == QueryFlags::OBSTACLE_MASK)
		{
			Obstacles* temporary = dynamic_cast<Obstacles*>(actors);
			temporary->FlipCollisions();
		}
	}
}

Actors* GameManager::GetSceneActor(const SceneNode& sceneNodeP)
{
	for (auto actor : mActors)
	{
		if (actor->GetSceneNode() == &sceneNodeP) return actor;
	}

	return nullptr;
}

bool GameManager::keyPressed(const KeyboardEvent& evt)
{
	Keycode key = evt.keysym.sym;

	if (key == SDLK_SPACE)
	{
		EndTurn();
	}

	return true;
}
