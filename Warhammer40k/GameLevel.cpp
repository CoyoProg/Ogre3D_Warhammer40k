#include "GameLevel.h"
#include <iostream>

#include "Ogre.h"
#include <OgreOverlayManager.h>

#include "GameManager.h"
#include "Actors.h"
#include "Player.h"
#include "TableTop.h"
#include "Figurines.h"
#include "Grid.h"
#include "Obstacles.h"

#include <array>
#include <random>
#include "QueryFlags.h"

using namespace Ogre;

void GameLevel::LoadEnvironment(Ogre::SceneManager &sceneManagerP, GameManager &gameEngineP, Grid &gridP)
{
	SetupLights(sceneManagerP);
	SetupBackgroundViewport(sceneManagerP);
}

void GameLevel::SetupBackgroundViewport(Ogre::SceneManager& sceneManagerP)
{
	Ogre::Viewport* mainViewport = sceneManagerP.getCamera("mainCamera")->getViewport();

	/* Set the background color for the main viewport */
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	mainViewport->setBackgroundColour(fadeColour);
	sceneManagerP.setFog(Ogre::FOG_LINEAR, fadeColour, 0, 2000, 10000);
}

void GameLevel::SetupLights(Ogre::SceneManager& sceneManagerP)
{
	// ===== LIGHT ==== 
	/* Set ambient light for the Scene Manager */
	sceneManagerP.setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	sceneManagerP.setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
	sceneManagerP.setShadowFarDistance(50);

	/* Create the light */
	Light* light = sceneManagerP.createLight("MainLight");
	light->setType(Light::LightTypes::LT_DIRECTIONAL);
	light->setDiffuseColour(1, 1, 1);
	light->setSpecularColour(1, 1, 1);

	/* Create a SceneNode for the light, and attach the new light */
	SceneNode* lightNode = sceneManagerP.getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setDirection(1, -2, -1);

	/* SkyBox */
	sceneManagerP.setSkyBox(true, "Examples/CloudyNoonSkyBox");
}

void GameLevel::SpawnActors(GameManager& gameEngineP, Grid& gridP)
{
	/* Blue carpet */
	Entity* planeEntity = gameEngineP.GetSceneManager().createEntity("Plane", SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Plateau.tapis");
	planeEntity->setQueryFlags((QueryFlags::OBSTACLE_MASK));

	SceneNode* planeNode = gameEngineP.GetSceneManager().getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(Vector3(0, -12, -100));
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(6, 4, 1));

	/* Create a random number generator */
	std::random_device rd;							// Obtain a random seed
	std::mt19937 gen(rd());							// Initialize the Mersenne Twister generator
	std::uniform_int_distribution<> random40(0, 40);	// Generate numbers between 0 and 40
	std::uniform_int_distribution<> random3(1, 3);		// Generate numbers between 1 and 3

	// WIP
	// TODO: REFACTOR BUIDLINGS + ADD MESH WITH TEXTURES
	int count = 0;
	for (int i = 1; i < 115; i++)
	{
		for (int j = 1; j < 75; j++)
		{
			/* PLAYER ONE */
			if (j > 25 && j < 55 &&
				i > 10 && i < 30)
			{
				if (j > 30 && j < 50 &&
					i > 15 && i < 25)
				{
					if (i % 4 != 0 || j % 4 != 0) continue;

					count++;

					const std::string& uniqueID = std::to_string(count);
					std::string entityName = "Space Marine " + uniqueID;
					std::string nodeName = "Node " + uniqueID;

					Figurines* figurines = new Figurines(gameEngineP, entityName, nodeName, 1);
					gameEngineP.AddActor(figurines);

					Vector3 newPosition = gridP.GetWorldPosition(Vector2(j, i));
					figurines->SetPosition(newPosition);
					figurines->SetYawRotation(Degree(180));
					figurines->SetMaterial("LowPolyMarine.blue");
				}

				continue;
			}

			/* PLAYER TWO */
			if (j > 25 && j < 55 &&
				i > 90 && i < 110)
			{
				if (j > 30 && j < 50 &&
					i > 95 && i < 105)
				{
					if (i % 4 != 0 || j % 4 != 0) continue;

					count++;
					const std::string& uniqueID = std::to_string(count);
					std::string entityName = "Space Marine " + uniqueID;
					std::string nodeName = "Node " + uniqueID;

					Figurines* figurines = new Figurines(gameEngineP, entityName, nodeName, 2);
					gameEngineP.AddActor(figurines);

					Vector3 newPosition = gridP.GetWorldPosition(Vector2(j, i));
					figurines->SetPosition(newPosition);
					figurines->SetYawRotation(Degree(0));
				}

				continue;
			}

			/* NOTHING IN THE CENTER AREA */
			if (j > 25 && j < 55 &&
				i > 45 && i < 75) continue;

			/* NOTHING IN THE LEFT AREA */
			if (j > 20)
			{
				/* BUILDINGS */
				int random_number = random40(gen);

				if (random_number != 1) continue;

				random_number = random3(gen);

				const std::string& uniqueID = "ID_" + std::to_string(i) + "_" + std::to_string(j);
				Obstacles* obstacle = new Obstacles(
					gameEngineP,
					Vector2(j, i),
					Vector3(1 + random_number, 5, 1 + random_number),
					uniqueID);
				gameEngineP.AddActor(obstacle);
			}
		}
	}
}
