#include "GameLevel.h"
#include <iostream>

#include "Ogre.h"

#include "GameEngine.h"
#include "Actors.h"
#include "Player.h"
#include "TableTop.h"
#include "Figurines.h"
#include "Grid.h"
#include "Obstacles.h"

using namespace Ogre;

void GameLevel::LoadLevel(GameEngine &gameEngineP)
{
	SceneManager &sceneManager = *gameEngineP.GetSceneManager();
	sceneManager.setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
	sceneManager.setShadowFarDistance(50);

	/* Create the Grid */
	Grid *grid = new Grid(gameEngineP);
	gameEngineP.SetGrid(grid);

	/* Player need to be initialize first for the main camera */
	// ===== Player ==== 
	Player *player = new Player(gameEngineP);

	TableTop *tabletop = new TableTop(gameEngineP);
	gameEngineP.AddActor(tabletop);
	gameEngineP.tabletop = tabletop;

	/* Import Custom mesh */
	Ogre::MeshPtr mMesh = MeshManager::getSingleton().load("LowPolyMarine.mesh", "AssetsGroup");
	mMesh->buildEdgeList();

	/* Player One */
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
	    for (int j = 0; j < 3; j++)
	    {
	        count++;
	        std::string entityName = "Space Marine " + std::to_string(count);
	        std::string nodeName = "Node " + std::to_string(count);
	
	        Figurines *figurines = new Figurines(gameEngineP, entityName, nodeName, 1);
	        gameEngineP.AddActor(figurines);

			Vector3 newPosition = grid->GetWorldPosition(Vector2(i * (2 * GRID_MULTIPLICATEUR), j * (2 * GRID_MULTIPLICATEUR)));
	        figurines->SetPosition(newPosition + Vector3(45, 0, -125)); // -25
			figurines->SetYawRotation(Degree(180));
			figurines->SetMaterial("LowPolyMarine.blue");
	    }
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			count++;
			std::string entityName = "Space Marine " + std::to_string(count);
			std::string nodeName = "Node " + std::to_string(count);
	
			Figurines *figurines = new Figurines(gameEngineP, entityName, nodeName, 1);
			gameEngineP.AddActor(figurines);

			Vector3 newPosition = grid->GetWorldPosition(Vector2(i* (2 * GRID_MULTIPLICATEUR), j * (2 * GRID_MULTIPLICATEUR)));
			figurines->SetPosition(newPosition + Vector3(135 - 3*10, 0, -125)); // -25
			figurines->SetYawRotation(Degree(180));
			figurines->SetMaterial("LowPolyMarine.blue");
		}
	}

	/* Player 2 */
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			count++;
			std::string entityName = "Space Marine " + std::to_string(count);
			std::string nodeName = "Node" + std::to_string(count);

			Figurines *figurines = new Figurines(gameEngineP, entityName, nodeName, 2);
			gameEngineP.AddActor(figurines);

			Vector3 newPosition = grid->GetWorldPosition(Vector2(i * (2 * GRID_MULTIPLICATEUR), GRID_SIZE_Z - j * (2 * GRID_MULTIPLICATEUR)));
			figurines->SetPosition(newPosition + Vector3(45, 0, 120)); // 25
			figurines->SetYawRotation(Degree(0));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			count++;
			std::string entityName = "Space Marine " + std::to_string(count);
			std::string nodeName = "Node" + std::to_string(count);

			Figurines* figurines = new Figurines(gameEngineP, entityName, nodeName, 2);
			gameEngineP.AddActor(figurines);

			Vector3 newPosition = grid->GetWorldPosition(Vector2(i * (2 * GRID_MULTIPLICATEUR), GRID_SIZE_Z - j * (2 * GRID_MULTIPLICATEUR)));
			figurines->SetPosition(newPosition + Vector3(135 - 3 * 10, 0, 120)); // 25
			figurines->SetYawRotation(Degree(0));
		}
	}

	LoadEnvironment(sceneManager, gameEngineP, grid);
}

void GameLevel::LoadEnvironment(Ogre::SceneManager &sceneManagerP, GameEngine &gameEngineP, Grid *gridP)
{
	// ===== LIGHT ==== 
	/* Create the light */
	Light *light = sceneManagerP.createLight("MainLight");
	light->setType(Light::LightTypes::LT_DIRECTIONAL);
	light->setDiffuseColour(1, 1, 1);
	light->setSpecularColour(1, 1, 1);

	/* Create a SceneNode for the light, and attach the new light */
	SceneNode *lightNode = sceneManagerP.getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setDirection(1, -2, -1);

	/* SkyBox */
	sceneManagerP.setSkyBox(true, "Examples/CloudyNoonSkyBox");                        // SkyBox

	/* Fog */
	Ogre::Viewport *mainViewport = sceneManagerP.getCamera("mainCamera")->getViewport();

	// Set the background color for the main viewport
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	mainViewport->setBackgroundColour(fadeColour);
	sceneManagerP.setFog(Ogre::FOG_LINEAR, fadeColour, 0, 2000, 10000);

	/* Buildings */
	Obstacles *obstacle = new Obstacles(
		gameEngineP, 
		Vector2(20, 25), 
		Vector3(7,5,14), 
		"01");
	gameEngineP.AddActor(obstacle);

	Obstacles *obstacle2 = new Obstacles(
		gameEngineP,
		Vector2(30, 35),
		Vector3(14, 4, 3),
		"02");
	gameEngineP.AddActor(obstacle2);
	
	
	Obstacles *obstacle3 = new Obstacles(
		gameEngineP,
		Vector2(5, 20),
		Vector3(5, 5, 5),
		"03");
	gameEngineP.AddActor(obstacle3);
}