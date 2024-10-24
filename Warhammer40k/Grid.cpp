#include "Grid.h"
#include <iostream>

Grid::Grid(GameEngine& gameEngineP) :
	mGameEngine(gameEngineP)
{
	gameEngineP.SetGrid(this);
	CreateGrid();
}

Grid::~Grid()
{

}

void Grid::CreateGrid()
{
	SceneManager &sceneManager = *mGameEngine.GetSceneManager();

	int count = 0;
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int z = 0; z < GRID_SIZE_Z; z++)
		{
			count++;
			CreateTiles(sceneManager, count, x, z);
		}
	}

	mGrid[0][GRID_SIZE_Z-1]->SetTile(TILE_MOVEMENT_SELECTED);
	mGrid[GRID_SIZE_X-1][GRID_SIZE_Z-1]->SetTile(TILE_MOVEMENT_SELECTED);

	mGrid[0][0]->SetTile(TILE_MOVEMENT_SELECTED);
	mGrid[GRID_SIZE_X-1][0]->SetTile(TILE_MOVEMENT_SELECTED);

	mGrid[39][0]->SetTile(TILE_MOVEMENT_SELECTED);
	mGrid[40][0]->SetTile(TILE_MOVEMENT_SELECTED);
}

void Grid::CreateTiles(Ogre::SceneManager &sceneManagerP, int countP, int coordXP, int coordZP)
{
	std::string entityName = "Tile " + std::to_string(countP);
	std::string nodeName = "TileNode " + std::to_string(countP);

	Entity *planeEntity = sceneManagerP.createEntity(entityName, SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Empty");
	SceneNode *planeNode = sceneManagerP.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(Vector3(coordXP * GRID_CELL_SIZE + GRID_CELL_SIZE / 2, 0.11f, -coordZP * GRID_CELL_SIZE - GRID_CELL_SIZE / 2) + GRID_OFFSET);
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(.025f / GRID_MULTIPLICATEUR, .025f / GRID_MULTIPLICATEUR, 1.f));

	mGrid[coordXP][coordZP] = new Tile(planeEntity, 0, Vector2(coordXP, coordZP));
}

void Grid::SetTileCollision(int coordXP, int coordZP, int enumTypeP)
{
	mGrid[coordXP][-coordZP]->SetTile(enumTypeP);
}

Tile* Grid::GetTile(Vector3 positionP)
{
	Vector2 tileCoords = GetGridCoords(positionP);
	int coordX = tileCoords.x;
	int coordZ = tileCoords.y;

	return mGrid[coordX][-coordZ];
}

std::vector<Tile*> Grid::GetNeighboursTiles(Tile *currentTileP)
{
	std::vector<Tile*> neighbours;
	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			if (x == 0 && z == 0)
				continue;

			int checkX = currentTileP->gridCoordinates.x + x;
			int checkZ = currentTileP->gridCoordinates.y + z;

			if (checkX >= 0 && checkX < GRID_SIZE_X &&
				checkZ >= 0 && checkZ < GRID_SIZE_Z)
			{
				neighbours.emplace_back(mGrid[checkX][checkZ]);
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

Vector3 Grid::GetWorldPosition(Vector2 gridCoordsP)
{
	Vector3 position;
	position.x = gridCoordsP.x * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
	position.y = 0;
	position.z = -gridCoordsP.y * GRID_CELL_SIZE - GRID_CELL_SIZE / 2;

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
			mGrid[x][z]->SetTile(TILE_EMPTY);
		}
	}
}
