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
	void GetMovementGrid(Vector3 startPositionP, int movementPointP, int mTileTypeP = 1);
	void HideMovementGrid(bool isSelectedP = false);

	void RetracePath(Tile *startTileP, Tile *targetTileP);
	std::vector<TurnThreshold*> GetTurnPath();

	std::vector<Vector3> lookPoints;
	std::vector<TurnThreshold*> turnBoundaries;
	int finishLineIndex = 0;
	int turnDst = 5;
	SceneManager *sceneManager = nullptr;

	/* DEBUG */
	void DrawLines()
	{
		for (auto lines : turnBoundaries)
		{
			lines->DrawLine(sceneManager);
		}
	}
	/* DEBUG */

	int totalCost = 0;

private:
	Grid &mGrid;

	std::vector<Tile*> mMovementGrid;
	std::map<Tile*, Tile*> mParentSet;
	std::vector<Vector3> mFinalPath;
	Vector3 mStartPosition;
	Vector3 mTargetPosition;

	int GetDistance(const Tile& tileAP, const Tile& tileBP);
};

