#pragma once
#include "GameEngine.h"

const int GRID_MULTIPLICATEUR = 2;

const int TILE_OBSTACLE = 2; // RED
const int TILE_VALID = 1;	 // GREEN
const int TILE_EMPTY = 0;	 // NOT VISIBLE

const int GRID_SIZE_Z = 60 * GRID_MULTIPLICATEUR;
const int GRID_SIZE_X = 40 * GRID_MULTIPLICATEUR;

const float GRID_CELL_SIZE{ 5.f / GRID_MULTIPLICATEUR };
const Vector3 GRID_OFFSET{ -GRID_SIZE_X / 2 * GRID_CELL_SIZE, 0, GRID_SIZE_Z / 2 * GRID_CELL_SIZE };

class Tile
{
public:
	Tile(Entity* tileEntityP, int tileTypeP, Vector2 gridCoordinatesP) 
	{
		tileEntity = tileEntityP; 
		tileType = tileTypeP;

		gridCoordinates = gridCoordinatesP;
	}
	~Tile() {};

	void SetTile(int enumType)
	{
		switch (enumType)
		{
		case 0:
			tileEntity->setMaterialName("Tile_Empty");
			tileType = 0;
			break;
		case 1:
			tileEntity->setMaterialName("Tile_Valid");
			tileType = 1;
			break;
		case 2:
			tileEntity->setMaterialName("Tile_Obstacle");
			tileType = 2;
			break;
		}
	}

	int GetType() { return tileType; }
	int GetFCost() { return gCost + hCost; }
	int gCost{INT_MAX};
	int hCost{0};

	Vector2 gridCoordinates{ 0,0 };
	Tile* parentNode{ nullptr };
private:
	Entity* tileEntity;
	int tileType{ 0 };
};

class Grid
{
public:
	Grid(GameEngine& gameEngineP);
	~Grid();

	void SetTileMaterial(int coordX, int coordZ, int enumType);

	/* PathFinding */
	int GetTileType(int coordX, int coordZ) { return grid[coordX][-coordZ]->GetType(); }
	Tile* GetTile(Vector3 positionP);
	std::vector<Tile*> GetNeighboursTiles(Tile* currentTile);

	Vector2 GetGridCoords(Vector3 positionP);
	Vector3 GetWorldPosition(Vector2 gridCoords);
	Vector3 SnapToGrid(Vector3 positionP);

	void OnFlip();
private:
	GameEngine& m_gameEngine;

	/*Grid properties */
	float padding{ 1.f };
	Tile* grid[GRID_SIZE_X][GRID_SIZE_Z];

	void CreateGrid();
	void CreateTiles(Ogre::SceneManager& sceneManager, int count, int coordX, int coordZ);
};

