#pragma once
#include "GameEngine.h"

const int TILE_OBSTACLE = 1; // RED
const int TILE_FREE = 0;	 // GREEN

const int GRID_SIZE_Z = 30;
const int GRID_SIZE_X = 20;

const float GRID_CELL_SIZE{ 10.f };
const Vector3 GRID_OFFSET{ -90, 0, 250 }; // To put the grid where the TableTop is

class Tile
{
public:
	Tile(Entity* tileEntityP, int tileTypeP) 
	{
		tileEntity = tileEntityP; 
		tileType = tileTypeP;
	}
	~Tile() {};

	void SetTile()
	{
		tileEntity->setMaterialName("Tile_Obstacle");
	}
	int GetType() { return tileType; }
private:
	Entity* tileEntity;
	int tileType{ 0 };
};

class Grid
{
public:
	Grid(GameEngine& gameEngineP);
	~Grid();

	void SetTileMaterial(int coordX, int coordZ);
	int GetTileType(int coordX, int coordZ) { return grid[coordX][-coordZ]->GetType(); }

	Vector2 GetGridCoords(Vector3 positionP);
	Vector3 GetWorldCoords(Vector2 gridCoords);
	Vector3 SnapToGrid(Vector3 positionP);
private:
	GameEngine& m_gameEngine;

	/*Grid properties */
	float padding{ 1.f };
	Tile* grid[GRID_SIZE_X][GRID_SIZE_Z];

	void CreateGrid();
	void CreateTiles(Ogre::SceneManager& sceneManager, int count, int coordX, int coordZ);
};

