#include "GameEngine.h"

/* Ogre Related */
#include "OgreRTShaderSystem.h"
#include "OgreText.h"
#include <OgreFontManager.h>
#include <OgreTrays.h>

/* C++ Related */
#include <iostream>

/* My scripts */
#include "Actors.h"
#include "GameLevel.h"
#include "Player.h"
#include "TableTop.h"

#include "Grid.h"
#include "QueryFlags.h"
#include "Obstacles.h"

using namespace Ogre;

GameEngine::GameEngine() :
	tabletop(nullptr),
	m_SceneManager(nullptr),
	m_grid(nullptr),
	m_Player(nullptr)
{
}

void GameEngine::setup()
{
	// do not forget to call the base first
	ApplicationContext::setup();
	addInputListener(this);

	// get a pointer to the already created root
	Root* root = getRoot();
	m_SceneManager = root->createSceneManager();

	// Create an empty SceneNode
	centerOfWorldNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("CenterOfWorldNode");

	// Optionally, set the position, orientation, or scale of the empty SceneNode
	centerOfWorldNode->setPosition(Vector3(0, 0, 0));
	centerOfWorldNode->setOrientation(Ogre::Quaternion::IDENTITY);
	centerOfWorldNode->setScale(Vector3(1, 1, 1));


	// register our scene with the RTSS
	RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(m_SceneManager);

	/* Set ambient light for the Scene Manager */
	m_SceneManager->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

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

	/*Load Level */
	GameLevel::LoadLevel(*this);

	Ogre::OverlaySystem* pOverlaySystem = getOverlaySystem();
	m_SceneManager->addRenderQueueListener(pOverlaySystem);

	textItem = new OgreText;
	textItem->setText("Player One");

	textItem->setPos(0.5f, 0.9f);
	textItem->setCol(0.0f, 0.f, 0.7f, 1.f);
}

void GameEngine::Update(float deltaTimeP)
{
	for (auto& actors : m_Actors)
	{
		actors->Update(deltaTimeP);
	}

	if(m_Player)
		m_Player->Update(deltaTimeP);

	if (LastFlipTimer < TimeBetweenFlips)
		LastFlipTimer += deltaTimeP;

	flipTableTop(deltaTimeP);
}

void GameEngine::EndTurn()
{
	// NEEDS TO CHECK IF ALL THE ACTORS AREN'T MOVING

	if (LastFlipTimer < TimeBetweenFlips)
		return;

	LastFlipTimer = 0;

	isFlipping = true;
	m_Player->SwapPlayer();
	int playerTurns = m_Player->GetCurrentPlayer();

	if (playerTurns == 1)
	{
		textItem->setText("Player One");
		textItem->setCol(0.0f, 0.f, 0.7f, 1.f);
	}
	else
	{
		textItem->setText("Player Two");
		textItem->setCol(0.7f, 0.f, 0.0f, 1.f);
	}

	for (auto actors : m_Actors)
	{
		if (!actors->GetSceneNode())
			continue;

		SceneNode* currentParent = actors->GetSceneNode()->getParentSceneNode();
		if (currentParent)
			currentParent->removeChild(actors->GetSceneNode());

		centerOfWorldNode->addChild(actors->GetSceneNode());
	}
}

Actors* GameEngine::GetSceneActor(const SceneNode* sceneNodeP)
{
	for (auto actors : m_Actors)
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

void GameEngine::flipTableTop(float deltaTime)
{
	if (isFlipping)
	{
		if (flipProgress > -0.28f && !changeDirection)
		{
			flipProgress -= deltaTime;

			if (animationSpeed < 1.f)
				animationSpeed += deltaTime;
			else
				animationSpeed = 1.f;

		}
		else
		{
			changeDirection = true;
			flipProgress += deltaTime;

			if (animationSpeed < 5.f)
				animationSpeed += deltaTime * 6;
			else
				animationSpeed = 5.f;
		}

		/* Bump Height */
		//flipProgress += deltaTime;
		float translationY = sin(flipProgress * animationSpeed) * bumpHeight;
		Vector3 newPos = tabletop->GetSceneNode()->getPosition();
		newPos.y = translationY;
		tabletop->GetSceneNode()->setPosition(newPos);

		newPos = centerOfWorldNode->getPosition();
		newPos.y = translationY;
		centerOfWorldNode->setPosition(newPos);


		/* Rotation Angle */
		rotationAngle = sin(flipProgress * animationSpeed / 2) * 180;
		Quaternion orientation = Ogre::Quaternion::IDENTITY;
		orientation.FromAngleAxis(Degree(rotationAngle + 90 * flipFlop), Ogre::Vector3::UNIT_Y);
		tabletop->GetSceneNode()->setOrientation(orientation);

		if (flipFlop == -1)
			orientation.FromAngleAxis(Degree(rotationAngle + 180), Ogre::Vector3::UNIT_Y);
		else
			orientation.FromAngleAxis(Degree(rotationAngle), Ogre::Vector3::UNIT_Y);
		centerOfWorldNode->setOrientation(orientation);


		Quaternion currentOrientation = Ogre::Quaternion::IDENTITY;
		currentOrientation = tabletop->GetSceneNode()->getOrientation();
		// Check if the flip is complete
		if (rotationAngle > 179)
		{
			flipFlop = -flipFlop;
			flipProgress = 0.0f;

			/* Reset Table orientation & position */
			Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
			orientation.FromAngleAxis(Degree(90 * flipFlop), Ogre::Vector3::UNIT_Y);
			tabletop->GetSceneNode()->setOrientation(orientation);

			Vector3 finalPos = tabletop->GetSceneNode()->getPosition();
			tabletop->GetSceneNode()->setPosition(Vector3(finalPos.x, 0, finalPos.z));

			/* Reset Actors orientation & position */
			if (flipFlop == -1)
				orientation.FromAngleAxis(Degree(180), Ogre::Vector3::UNIT_Y);
			else
				orientation.FromAngleAxis(Degree(0), Ogre::Vector3::UNIT_Y);
			centerOfWorldNode->setOrientation(orientation);
			centerOfWorldNode->setPosition(Vector3(0, 0, 0));


			isFlipping = false;
			changeDirection = false;
			animationSpeed = 0.f;

			m_grid->OnFlip();

			for (auto actors : m_Actors)
			{
				if (!actors->GetEntity())
					continue;

				if(actors->GetEntity()->getQueryFlags() == QueryFlags::OBSTACLE_MASK)
				{
					Obstacles* temporary = dynamic_cast<Obstacles*>(actors);
					temporary->AddLineCollision(*m_grid, temporary->GetSceneNode()->_getDerivedPosition(), !temporary->isFlipped, 2 * GRID_MULTIPLICATEUR);
				}
			}
		}
	}
}
