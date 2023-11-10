#include "GameLevel.h"
#include <iostream>

#include "Ogre.h"

#include "GameEngine.h"
#include "Actors.h"
#include "Player.h"
#include "TableTop.h"
#include "Figurines.h"
#include "Grid.h"

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
	        figurines->SetPosition(Vector3(i * 10 + 10 / 2 +1, 0.75f, -j * 10 - 10 / 2 +1) + Vector3(-90, 0, 250));
	    }
	}

	LoadEnvironment(sceneManager);

	/* Create the Grid */
	Grid* grid = new Grid(gameEngineP);
	gameEngineP.SetGrid(grid);
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