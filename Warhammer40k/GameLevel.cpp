#include "GameLevel.h"
#include <iostream>

#include "Ogre.h"

#include "GameEngine.h"
#include "Actors.h"
#include "Player.h"
#include "TableTop.h"
#include "Figurines.h"

using namespace Ogre;

void GameLevel::LoadLevel(GameEngine& gameEngineP)
{
	SceneManager& sceneManager = *gameEngineP.GetSceneManager();
	sceneManager.setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
	sceneManager.setShadowFarDistance(100);

	/* Player need to be initialize first for the main camera */
	// ===== Player ==== 
	Player* player = new Player(gameEngineP);
	gameEngineP.AddActor(player);

	TableTop* tabletop = new TableTop(gameEngineP);
	gameEngineP.AddActor(tabletop);

	/* Import Custom mesh */
	Ogre::MeshPtr mMesh = MeshManager::getSingleton().load("LowPolyMarine.mesh", "AssetsGroup");
	mMesh->buildEdgeList();

	/* =========== JUST FOR DEBUG PURPOSE =========== */
	int count = 0;
	for (int i = 0; i < 10; i++)
	{
	    for (int j = 0; j < 10; j++)
	    {
	        count++;
	        std::string entityName = "Figurine " + std::to_string(count);
	        std::string nodeName = "Node " + std::to_string(count);
	
	        Figurines* figurines = new Figurines(sceneManager, entityName, nodeName);
	        gameEngineP.AddActor(figurines);
	        figurines->SetPosition(Vector3(i * 10 + 10 / 2, 0.75f, -j * 10 - 10 / 2) + Vector3(-90, 0, 250));
	    }
	}

	LoadEnvironment(sceneManager);


	////// GRID /////
	int gridSizeZ = 28;
	int gridSizeX = 18;
	int gridCellSize = 10;
	int padding = 1.f;

	const Vector3 offset(-90, 0, 250);

	count = 0;
	for (int i = 0; i < gridSizeX; i++)
	{
		for (int j = 0; j < gridSizeZ; j++)
		{
			count++;
			std::string entityName = "Plane " + std::to_string(count);
			std::string nodeName = "Node " + std::to_string(count);

			Entity* planeEntity = sceneManager.createEntity(entityName, SceneManager::PT_PLANE);
			planeEntity->setMaterialName("Tile_Valid");
			SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
			planeNode->attachObject(planeEntity);
			planeNode->setPosition(Vector3(i * gridCellSize + gridCellSize/2, 0.2f, -j * gridCellSize - gridCellSize / 2) + offset);
			planeNode->pitch(Degree(-90));
			planeNode->setScale(Vector3(.045f, .045f, 1.f));
		}
	}

	Entity* planeEntity = sceneManager.createEntity("BackgroundTile", SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Background");
	SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(Vector3(0,0.1f,105));
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(0.95f, 1.45f, 1.f));

	// Create a cube entity
	Ogre::Entity* cubeEntity = sceneManager.createEntity("CubeEntity", Ogre::SceneManager::PT_CUBE);
	// Create a scene node
	Ogre::SceneNode* cubeNode = sceneManager.getRootSceneNode()->createChildSceneNode("CubeNode");
	// Attach the cube entity to the scene node
	cubeNode->attachObject(cubeEntity);
	cubeNode->setPosition(Vector3(0, 0.75f, 0) + offset);
	cubeNode->setScale(0.01f, 0.01f, 0.01f);

	// Create a cube entity
	Ogre::Entity* cubeEntity2 = sceneManager.createEntity("CubeEntity2", Ogre::SceneManager::PT_CUBE);
	// Create a scene node
	Ogre::SceneNode* cubeNode2 = sceneManager.getRootSceneNode()->createChildSceneNode("CubeNode2");
	// Attach the cube entity to the scene node
	cubeNode2->attachObject(cubeEntity2);
	cubeNode2->setPosition(Vector3(10, 0.75f, -10) + offset); // i = 1, j = 1
	cubeNode2->setScale(0.01f, 0.01f, 0.01f);
}

void GameLevel::LoadEnvironment(Ogre::SceneManager& sceneManager)
{
	// ===== LIGHT ==== 
	/* Create the light */
	Light* light = sceneManager.createLight("MainLight");
	light->setType(Light::LightTypes::LT_DIRECTIONAL);
	light->setDiffuseColour(1, 1, 1);
	light->setSpecularColour(1, 1, 1);

	/* Create a SceneNode for the light, and attach the new light */
	SceneNode* lightNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setDirection(1, -2, -1);

	/* SkyBox */
	sceneManager.setSkyBox(true, "Examples/CloudyNoonSkyBox");                        // SkyBox

	/* Fog */
	Ogre::Viewport* mainViewport = sceneManager.getCamera("mainCamera")->getViewport();

	// Set the background color for the main viewport
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	mainViewport->setBackgroundColour(fadeColour);
	sceneManager.setFog(Ogre::FOG_LINEAR, fadeColour, 0, 2000, 10000);
}