#pragma once
#include "Components.h"
#include "TurnThreshold.h"

class Grid;
class Tile;

class PathFindingComponent : public Components
{
public:
	PathFindingComponent(GameEngine &gameEngineP);
	~PathFindingComponent();

	virtual void Update(float deltaTimeP) override;

	/* This function uses Dijkstra's algorithm to find all the tiles that are accessible. */
	void GetMovementGrid(const Vector3& startPositionP, int movementPointP, int tileTypeP = 1);
	void HideMovementGrid(bool isSelectedP = false);
	void RetracePath(Tile *startTileP, Tile *targetTileP);
	std::vector<TurnThreshold*> GetTurnPath();

	std::vector<Vector3> lookPoints;
	std::vector<TurnThreshold*> turnBoundaries;
	int finishLineIndex = 0;
	int turnDst = 5;
	int totalPathCost = 0;

	/* DEBUG */
	void DrawLines();

private:
	SceneManager& sceneManager;
	Grid &mGrid;

	std::vector<Tile*> mMovementGrid;
	std::map<Tile*, Tile*> mParentSet;
	std::vector<Vector3> mFinalPath;

	int GetDistance(const Tile& tileAP, const Tile& tileBP);
};

