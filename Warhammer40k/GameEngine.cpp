#include <windows.h>

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

GameEngine::GameEngine()
{
}

void GameEngine::setup()
{
	InitializeRenderer();
	InitializeGame();
}

void GameEngine::InitializeRenderer()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;

	mRoot->initialise(false);
	createWindow("Warhammer 40k TableTop", screenWidth, screenHeight);

	locateResources();
	initialiseRTShaderSystem();
	loadResources();

	mRoot->addFrameListener(this);
	mRenderWindow = ApplicationContext::getRenderWindow();

	mSceneManager = mRoot->createSceneManager();

	addInputListener(this);

	Ogre::OverlaySystem* pOverlaySystem = getOverlaySystem();
	mSceneManager->addRenderQueueListener(pOverlaySystem);

	// register our scene with the RTSS
	RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mSceneManager);

	/* Load mesh into ressources */
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

void GameEngine::InitializeGame()
{
	// Create an empty SceneNode
	mCenterOfWorldNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CenterOfWorldNode");

	// Optionally, set the position, orientation, or scale of the empty SceneNode
	mCenterOfWorldNode->setPosition(Vector3(0, 0, 0));
	mCenterOfWorldNode->setOrientation(Ogre::Quaternion::IDENTITY);
	mCenterOfWorldNode->setScale(Vector3(1, 1, 1));

	/* Set ambient light for the Scene Manager */
	mSceneManager->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	/*Load Level */
	GameLevel::LoadLevel(*this);

	mOverlayWidgets = new OgreText(*mRenderWindow);
	mOverlayWidgets->GetPlayerTextElement()->setCaption("Player One");
}

void GameEngine::Update(float deltaTimeP)
{
	for (auto& actors : mActors)
	{
		actors->Update(deltaTimeP);
	}

	if(mPlayer)
		mPlayer->Update(deltaTimeP);

	if (mLastFlipTimer < mTimeBetweenFlips)
		mLastFlipTimer += deltaTimeP;

	FlipTableTop(deltaTimeP);
}

void GameEngine::RemoveActor(Actors *actorP)
{
	// Find the iterator pointing to the element
	auto iter = std::find(mActors.begin(), mActors.end(), actorP);

	// Check if the element was found before erasing
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}

	delete actorP;
}

void GameEngine::EndTurn()
{
	// NEEDS TO CHECK IF ALL THE ACTORS AREN'T MOVING

	if (mLastFlipTimer < mTimeBetweenFlips)
		return;

	mLastFlipTimer = 0;

	mIsFlipping = true;
	mPlayer->OnEndTurn();
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

	for (auto actors : mActors)
	{
		SceneNode *actorNode = actors->GetSceneNode();
		
		if (!actorNode)
			continue;
		
		actors->OnEndTurn();
		
		SceneNode *parentNode = actorNode->getParentSceneNode();
		if (parentNode)
			parentNode->removeChild(actorNode);
		
		mCenterOfWorldNode->addChild(actorNode);
	}
}

Actors* GameEngine::GetSceneActor(const SceneNode *sceneNodeP)
{
	for (auto actors : mActors)
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
	if (!isGameLoaded)
		isGameLoaded = true;

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

bool GameEngine::keyPressed(const KeyboardEvent& evt)
{
	Keycode key = evt.keysym.sym;

	if (key == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}

	if (key == SDLK_SPACE)
	{
		EndTurn();
	}

	return true;
}

void GameEngine::FlipTableTop(float deltaTimeP)
{
	if (mIsFlipping)
	{
		if (mFlipProgress > -0.28f && !mChangeDirection)
		{
			mFlipProgress -= deltaTimeP;

			if (mAnimationSpeed < 1.f)
				mAnimationSpeed += deltaTimeP;
			else
				mAnimationSpeed = 1.f;

		}
		else
		{
			mChangeDirection = true;
			mFlipProgress += deltaTimeP;

			if (mAnimationSpeed < 5.f)
				mAnimationSpeed += deltaTimeP * 6;
			else
				mAnimationSpeed = 5.f;
		}

		/* Bump Height */
		//mFlipProgress += deltaTimeP;
		float translationY = sin(mFlipProgress * mAnimationSpeed) * mBumpHeight;
		Vector3 newPos = tabletop->GetSceneNode()->getPosition();
		newPos.y = translationY;
		tabletop->GetSceneNode()->setPosition(newPos);

		newPos = mCenterOfWorldNode->getPosition();
		newPos.y = translationY;
		mCenterOfWorldNode->setPosition(newPos);


		/* Rotation Angle */
		mRotationAngle = sin(mFlipProgress * mAnimationSpeed / 2) * 180;
		Quaternion orientation = Ogre::Quaternion::IDENTITY;
		orientation.FromAngleAxis(Degree(mRotationAngle + 90 * mFlipFlop), Ogre::Vector3::UNIT_Y);
		tabletop->GetSceneNode()->setOrientation(orientation);

		if (mFlipFlop == -1)
			orientation.FromAngleAxis(Degree(mRotationAngle + 180), Ogre::Vector3::UNIT_Y);
		else
			orientation.FromAngleAxis(Degree(mRotationAngle), Ogre::Vector3::UNIT_Y);
		mCenterOfWorldNode->setOrientation(orientation);


		Quaternion currentOrientation = Ogre::Quaternion::IDENTITY;
		currentOrientation = tabletop->GetSceneNode()->getOrientation();
		// Check if the flip is complete
		if (mRotationAngle > 179)
		{
			mFlipFlop = -mFlipFlop;
			mFlipProgress = 0.0f;

			/* Reset Table orientation & position */
			Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
			orientation.FromAngleAxis(Degree(90 * mFlipFlop), Ogre::Vector3::UNIT_Y);
			tabletop->GetSceneNode()->setOrientation(orientation);

			Vector3 finalPos = tabletop->GetSceneNode()->getPosition();
			tabletop->GetSceneNode()->setPosition(Vector3(finalPos.x, 0, finalPos.z));

			/* Reset Actors orientation & position */
			if (mFlipFlop == -1)
				orientation.FromAngleAxis(Degree(180), Ogre::Vector3::UNIT_Y);
			else
				orientation.FromAngleAxis(Degree(0), Ogre::Vector3::UNIT_Y);
			mCenterOfWorldNode->setOrientation(orientation);
			mCenterOfWorldNode->setPosition(Vector3(0, 0, 0));


			mIsFlipping = false;
			mChangeDirection = false;
			mAnimationSpeed = 0.f;

			mGrid->OnFlip();

			for (auto actors : mActors)
			{
				actors->OutEndTurn();

				if (!actors->GetObject())
					continue;

				if(actors->GetObject()->getQueryFlags() == QueryFlags::OBSTACLE_MASK)
				{
					Obstacles* temporary = dynamic_cast<Obstacles*>(actors);
					temporary->FlipCollisions();
				}
			}
		}
	}
}
