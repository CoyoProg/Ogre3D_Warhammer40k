#include "Grid.h"
#include <iostream>

#include "Obstacles.h"

Grid::Grid(GameEngine& gameEngineP) :
	m_gameEngine(gameEngineP)
{
	gameEngineP.SetGrid(this);
	CreateGrid();
}

Grid::~Grid()
{

}

void Grid::CreateGrid()
{
	SceneManager& sceneManager = *m_gameEngine.GetSceneManager();

	int count = 0;
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int z = 0; z < GRID_SIZE_Z; z++)
		{
			count++;
			CreateTiles(sceneManager, count, x, z);
		}
	}

	/* ======================== DEBUG ========================*/
	/* Create a background under the tiles */
	Entity* planeEntity = sceneManager.createEntity("BackgroundTile", SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Background");
	SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(GRID_OFFSET + Vector3(50, 0.1f, -50));
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(.50f, .50f, 1.f));
	
	/* Create a background under the tiles */
	Entity* planeEntity2 = sceneManager.createEntity("BackgroundTile2", SceneManager::PT_PLANE);
	planeEntity2->setMaterialName("Tile_Background");
	SceneNode* planeNode2 = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode2->attachObject(planeEntity2);
	planeNode2->setPosition(GRID_OFFSET + Vector3(150, 0.1f, -50));
	planeNode2->pitch(Degree(-90));
	planeNode2->setScale(Vector3(.50f, .50f, 1.f));

	Obstacles* obstacle = new Obstacles(m_gameEngine);
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
	planeNode->setPosition(Vector3(coordX * GRID_CELL_SIZE + GRID_CELL_SIZE / 2, 0.11f, -coordZ * GRID_CELL_SIZE - GRID_CELL_SIZE / 2) + GRID_OFFSET);
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(.045f, .045f, 1.f));

	grid[coordX][coordZ] = new Tile(planeEntity, 0);
}

void Grid::SetTileMaterial(int coordX, int coordZ)
{
	grid[coordX][-coordZ]->SetTile();
}

Vector2 Grid::GetGridCoords(Vector3 positionP)
{
	Vector3 position = positionP - GRID_OFFSET;
	int coordX = position.x / GRID_CELL_SIZE;
	int coordZ = position.z / GRID_CELL_SIZE;

	return Vector2(coordX, coordZ);
}

Vector3 Grid::GetWorldCoords(Vector2 gridCoords)
{
	Vector3 position;
	position.x = gridCoords.x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
	position.z = gridCoords.y * GRID_CELL_SIZE - GRID_CELL_SIZE / 2;

	position += GRID_OFFSET;

	return position;
}

Vector3 Grid::SnapToGrid(Vector3 positionP)
{
	/* Calculate the Grid Coordinate of a position */
	Vector2 gridCoords = GetGridCoords(positionP);
	Vector3 position = GetWorldCoords(gridCoords);

	return position;
}
