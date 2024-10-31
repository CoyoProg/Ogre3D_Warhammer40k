#ifdef _WIN32 
#include <windows.h>
#endif

#include "GameManager.h"
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

GameManager::GameManager()
{
}

void GameManager::setup()
{
	InitializeRenderer();
	InitializeGame();
}

void GameManager::InitializeRenderer()
{
#ifdef _WIN32 
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;
#endif

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

	/* register our scene with the RTSS */
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

void GameManager::InitializeGame()
{
	/* Create the center of the Game Node */
	mCenterOfWorldNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CenterOfWorldNode");

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

	if (mFlipAnimation.flipTimer < mFlipAnimation.flipsCooldown)
	{
		mFlipAnimation.flipTimer += deltaTimeP;
	}

	FlipTableTop(deltaTimeP);
}

void GameManager::RemoveActor(Actors *actorP)
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

	if (mFlipAnimation.flipTimer < mFlipAnimation.flipsCooldown)
		return;

	mFlipAnimation.flipTimer = 0;

	mFlipAnimation.isFlipping = true;
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

Actors* GameManager::GetSceneActor(const SceneNode &sceneNodeP)
{
	for (auto actor : mActors)
	{
		if (actor->GetSceneNode() == &sceneNodeP)
		{
			return actor;
		}
	}

	return nullptr;
}

bool GameManager::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (!isGameLoaded)
		isGameLoaded = true;

	/* Calculate the delta time (time since the last frame) */
	float deltaTime = fe.timeSinceLastFrame;

	/* Handle game logic and update here */
	Update(deltaTime);

	bool ret = ApplicationContext::frameRenderingQueued(fe);


	if (!ProcessUnbufferedInput(fe))
		return false;

	return ret;
}

bool GameManager::ProcessUnbufferedInput(const Ogre::FrameEvent& fe)
{
	return true;
}

bool GameManager::keyPressed(const KeyboardEvent& evt)
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

void GameManager::FlipTableTop(float deltaTimeP)
{
	if (mFlipAnimation.isFlipping)
	{
		if (mFlipAnimation.animationProgress > -0.28f && !mFlipAnimation.shouldChangeDir)
		{
			mFlipAnimation.animationProgress -= deltaTimeP;

			if (mFlipAnimation.animationSpeed < 1.f)
				mFlipAnimation.animationSpeed += deltaTimeP;
			else
				mFlipAnimation.animationSpeed = 1.f;

		}
		else
		{
			mFlipAnimation.shouldChangeDir = true;
			mFlipAnimation.animationProgress += deltaTimeP;

			if (mFlipAnimation.animationSpeed < 5.f)
				mFlipAnimation.animationSpeed += deltaTimeP * 6;
			else
				mFlipAnimation.animationSpeed = 5.f;
		}

		/* Bump Height */
		float translationY = sin(mFlipAnimation.animationProgress * mFlipAnimation.animationSpeed) * mFlipAnimation.bumpHeight;
		Vector3 newPos = tabletop->GetSceneNode()->getPosition();
		newPos.y = translationY;
		tabletop->GetSceneNode()->setPosition(newPos);

		newPos = mCenterOfWorldNode->getPosition();
		newPos.y = translationY;
		mCenterOfWorldNode->setPosition(newPos);


		/* Rotation Angle */
		mFlipAnimation.rotationAngle = sin(mFlipAnimation.animationProgress * mFlipAnimation.animationSpeed / 2) * 180;
		Quaternion orientation = Ogre::Quaternion::IDENTITY;
		orientation.FromAngleAxis(Degree(mFlipAnimation.rotationAngle + 90 * mFlipAnimation.flipFlop), Ogre::Vector3::UNIT_Y);
		tabletop->GetSceneNode()->setOrientation(orientation);

		if (mFlipAnimation.flipFlop == -1)
			orientation.FromAngleAxis(Degree(mFlipAnimation.rotationAngle + 180), Ogre::Vector3::UNIT_Y);
		else
			orientation.FromAngleAxis(Degree(mFlipAnimation.rotationAngle), Ogre::Vector3::UNIT_Y);
		mCenterOfWorldNode->setOrientation(orientation);


		Quaternion currentOrientation = Ogre::Quaternion::IDENTITY;
		currentOrientation = tabletop->GetSceneNode()->getOrientation();
		/* Check if the flip is complete */
		if (mFlipAnimation.rotationAngle > 179)
		{
			mFlipAnimation.flipFlop = -mFlipAnimation.flipFlop;
			mFlipAnimation.animationProgress = 0.0f;

			/* Reset Table orientation & position */
			Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
			orientation.FromAngleAxis(Degree(90 * mFlipAnimation.flipFlop), Ogre::Vector3::UNIT_Y);
			tabletop->GetSceneNode()->setOrientation(orientation);

			Vector3 finalPos = tabletop->GetSceneNode()->getPosition();
			tabletop->GetSceneNode()->setPosition(Vector3(finalPos.x, 0, finalPos.z));

			/* Reset Actors orientation & position */
			if (mFlipAnimation.flipFlop == -1)
				orientation.FromAngleAxis(Degree(180), Ogre::Vector3::UNIT_Y);
			else
				orientation.FromAngleAxis(Degree(0), Ogre::Vector3::UNIT_Y);
			mCenterOfWorldNode->setOrientation(orientation);
			mCenterOfWorldNode->setPosition(Vector3(0, 0, 0));


			mFlipAnimation.isFlipping = false;
			mFlipAnimation.shouldChangeDir = false;
			mFlipAnimation.animationSpeed = 0.f;

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
