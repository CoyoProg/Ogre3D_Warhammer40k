#pragma once
#include "GameEngine.h"

constexpr int GRID_MULTIPLICATEUR = 2;
	  
constexpr int TILE_MOVEMENT_BLEND = 5;			// SOFT GREEN & RED
constexpr int TILE_MOVEMENT_ENEMY = 4;			// SOFT RED
constexpr int TILE_MOVEMENT_MOUSEOVER = 3;		// SOFT GREEN
constexpr int TILE_OBSTACLE = 2;				// RED
constexpr int TILE_MOVEMENT_SELECTED = 1;		// GREEN
constexpr int TILE_EMPTY = 0;					// NOT VISIBLE

constexpr int GRID_SIZE_Z = 60 * GRID_MULTIPLICATEUR;
constexpr int GRID_SIZE_X = 40 * GRID_MULTIPLICATEUR;

constexpr float GRID_CELL_SIZE{ 5.f / GRID_MULTIPLICATEUR };
const Vector3 GRID_OFFSET{ -GRID_SIZE_X * 0.5f * GRID_CELL_SIZE, 0, GRID_SIZE_Z * 0.5f * GRID_CELL_SIZE };

class Tile
{
public:
	Tile(Entity* tileEntityP, int tileTypeP, Vector2 gridCoordinatesP) 
	{
		mTileEntity = tileEntityP; 
		mTileType = tileTypeP;

		gridCoordinates = gridCoordinatesP;
	}
	~Tile() {};

	void SetTile(int enumTypeP)
	{
		switch (enumTypeP)
		{
		case 0:
			mTileEntity->setMaterialName("Tile_Empty");
			mTileType = 0;
			break;
		case 1:
			mTileEntity->setMaterialName("Tile_Valid");
			mTileType = 1;
			break;
		case 2:
			//mTileEntity->setMaterialName("Tile_Obstacle");
			mTileEntity->setMaterialName("Tile_Empty");
			mTileType = 2;
			break;
		case 3:
			mTileEntity->setMaterialName("Tile_SoftValid");
			mTileType = 3;
			break;
		case 4:
			mTileEntity->setMaterialName("Tile_Enemy");
			mTileType = 4;
			break;
		case 5:
			mTileEntity->setMaterialName("Tile_Blend");
			mTileType = 5;
			break;
		}
	}

	int GetType() { return mTileType; }
	int GetFCost() { return gCost + hCost; }
	int gCost = INT_MAX;
	int hCost = 0;

	Vector2 gridCoordinates = Vector2(0);
	Tile *parentNode = nullptr;

private:
	Entity *mTileEntity;
	int mTileType{ 0 };
};

class Grid
{
public:
	Grid(GameEngine &gameEngineP);
	~Grid();

	void SetTileCollision(int coordXP, int coordZP, int enumTypeP);

	/* PathFinding */
	int GetTileType(int coordXP, int coordZP) { return mGrid[coordXP][-coordZP]->GetType(); }
	Tile* GetTile(Vector3 positionP);
	std::vector<Tile*> GetNeighboursTiles(Tile *currentTileP);

	Vector2 GetGridCoords(Vector3 positionP);
	Vector3 GetWorldPosition(Vector2 gridCoordsP);
	Vector3 SnapToGrid(Vector3 positionP);

	void OnFlip();

private:
	GameEngine &mGameEngine;

	/*Grid properties */
	float mPadding = 1.f;
	Tile* mGrid[GRID_SIZE_X][GRID_SIZE_Z];

	void CreateGrid();
	void CreateTiles(Ogre::SceneManager& sceneManagerP, int countP, int coordXP, int coordZP);
};

