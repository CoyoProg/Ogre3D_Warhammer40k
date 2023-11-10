#pragma once
#include "GameEngine.h"

const int TILE_OBSTACLE = 1; // RED
const int TILE_FREE = 0;	 // GREEN

const int GRID_SIZE_Z = 30;
const int GRID_SIZE_X = 20;

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

	void SetTile(int coordX, int coordZ);
	int GetTileType(int coordX, int coordZ) { return grid[coordX][-coordZ]->GetType(); }
private:
	/*Grid properties */
	float gridCellSize{ 10.f };
	float padding{ 1.f };

	Tile* grid[GRID_SIZE_X][GRID_SIZE_Z];
	const Vector3 offset{ -90, 0, 250 }; // To put the grid where the TableTop is

	void CreateGrid();
	void CreateTiles(Ogre::SceneManager& sceneManager, int count, int coordX, int coordZ);

	GameEngine& m_gameEngine;
};

