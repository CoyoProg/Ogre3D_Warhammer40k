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
	//Obstacles* obstacle = new Obstacles(m_gameEngine, GetWorldPosition(Vector2(15, 30)), "01", true);
	//m_gameEngine.AddActor(obstacle);
	//
	//Obstacles* obstacle2 = new Obstacles(m_gameEngine, GetWorldPosition(Vector2(15, 26)), "02", true);
	//m_gameEngine.AddActor(obstacle2);
	//
	//Obstacles* obstacle3 = new Obstacles(m_gameEngine, GetWorldPosition(Vector2(17.5, 32)), "03", false);
	//m_gameEngine.AddActor(obstacle3);
	/* ======================== DEBUG ========================*/
}

void Grid::CreateTiles(Ogre::SceneManager& sceneManager, int count, int coordX, int coordZ)
{
	std::string entityName = "Tile " + std::to_string(count);
	std::string nodeName = "TileNode " + std::to_string(count);

	Entity* planeEntity = sceneManager.createEntity(entityName, SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Empty");
	SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(Vector3(coordX * GRID_CELL_SIZE + GRID_CELL_SIZE / 2, 0.11f, -coordZ * GRID_CELL_SIZE - GRID_CELL_SIZE / 2) + GRID_OFFSET);
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(.025f / GRID_MULTIPLICATEUR, .025f / GRID_MULTIPLICATEUR, 1.f));

	grid[coordX][coordZ] = new Tile(planeEntity, 0, Vector2(coordX, coordZ));
}

void Grid::SetTileMaterial(int coordX, int coordZ, int enumType)
{
	grid[coordX][-coordZ]->SetTile(enumType);
}

Tile* Grid::GetTile(Vector3 positionP)
{
	Vector2 tileCoords = GetGridCoords(positionP);
	int coordX = tileCoords.x;
	int coordZ = tileCoords.y;

	return grid[coordX][-coordZ];
}

std::vector<Tile*> Grid::GetNeighboursTiles(Tile* currentTile)
{
	std::vector<Tile*> neighbours;
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			if (x == 0 && z == 0)
				continue;

			int checkX = currentTile->gridCoordinates.x + x;
			int checkZ = currentTile->gridCoordinates.y + z;

			if (checkX >= 0 && checkX < GRID_SIZE_X &&
				checkZ >= 0 && checkZ < GRID_SIZE_Z)
			{
				neighbours.emplace_back(grid[checkX][checkZ]);
			}
		}
	}

	return neighbours;
}

Vector2 Grid::GetGridCoords(Vector3 positionP)
{
	Vector3 position = positionP - GRID_OFFSET;
	int coordX = position.x / GRID_CELL_SIZE;
	int coordZ = position.z / GRID_CELL_SIZE;

	return Vector2(coordX, coordZ);
}

Vector3 Grid::GetWorldPosition(Vector2 gridCoords)
{
	Vector3 position;
	position.x = gridCoords.x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
	position.y = 0;
	position.z = -gridCoords.y * GRID_CELL_SIZE - GRID_CELL_SIZE / 2;

	position += GRID_OFFSET;

	return position;
}

Vector3 Grid::SnapToGrid(Vector3 positionP)
{
	/* Calculate the Grid Coordinate of a position */
	Vector2 gridCoords = GetGridCoords(positionP);
	Vector3 position = GetWorldPosition(gridCoords);

	return position;
}

void Grid::OnFlip()
{
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int z = 0; z < GRID_SIZE_Z; z++)
		{
			grid[x][z]->SetTile(TILE_EMPTY);
		}
	}
}
