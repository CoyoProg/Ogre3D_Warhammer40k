#include "Grid.h"

#include <iostream>

Grid::Grid(GameEngine& gameEngineP) :
	m_gameEngine(gameEngineP)
{
	CreateGrid();
}

Grid::~Grid()
{

}

void Grid::CreateGrid()
{
	SceneManager& sceneManager = *m_gameEngine.GetSceneManager();

	int count = 0;
	for (int i = 0; i < GRID_SIZE_X; i++)
	{
		for (int j = 0; j < GRID_SIZE_Z; j++)
		{
			count++;
			CreateTiles(sceneManager, count, i, j);
		}
	}

	/* ======================== DEBUG ========================*/
	/* Create a background under the tiles */
	Entity* planeEntity = sceneManager.createEntity("BackgroundTile", SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Background");
	SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(offset + Vector3(50, 0.1f, -50));
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(.50f, .50f, 1.f));
	
	/* Create a background under the tiles */
	Entity* planeEntity2 = sceneManager.createEntity("BackgroundTile2", SceneManager::PT_PLANE);
	planeEntity2->setMaterialName("Tile_Background");
	SceneNode* planeNode2 = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode2->attachObject(planeEntity2);
	planeNode2->setPosition(offset + Vector3(150, 0.1f, -50));
	planeNode2->pitch(Degree(-90));
	planeNode2->setScale(Vector3(.50f, .50f, 1.f));

	// Create a cube entity
	Ogre::Entity* cubeEntity = sceneManager.createEntity("CubeEntity", Ogre::SceneManager::PT_CUBE);
	// Create a scene node
	Ogre::SceneNode* cubeNode = sceneManager.getRootSceneNode()->createChildSceneNode("CubeNode");
	// Attach the cube entity to the scene node
	cubeNode->attachObject(cubeEntity);
	cubeNode->setScale(.075f, .075f, .075f);
	cubeNode->setPosition(offset + Vector3(12 * gridCellSize + gridCellSize / 2, 2.f, -5 * gridCellSize - gridCellSize / 2));


	/* Calculate the Grid Coordinate of a position */
	Vector3 position = cubeNode->getPosition() - offset;
	int coordX = position.x / gridCellSize;
	int coordZ = position.z / gridCellSize;
	std::cout << "CoordX: " << coordX << " || CoordZ: " << coordZ << std::endl;

	SetTile(coordX, coordZ);
	/* ======================== DEBUG ========================*/
}

void Grid::CreateTiles(Ogre::SceneManager& sceneManager, int count, int coordX, int coordZ)
{
	std::string entityName = "Tile " + std::to_string(count);
	std::string nodeName = "TileNode " + std::to_string(count);

	Entity* planeEntity = sceneManager.createEntity(entityName, SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Valid");
	SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(Vector3(coordX * gridCellSize + gridCellSize / 2, 0.11f, -coordZ * gridCellSize - gridCellSize / 2) + offset);
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(.045f, .045f, 1.f));

	grid[coordX][coordZ] = new Tile(planeEntity, 0);
}

void Grid::SetTile(int coordX, int coordZ)
{
	grid[coordX][-coordZ]->SetTile();
}
