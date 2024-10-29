#pragma once
#include "Components.h"
#include "TurnThreshold.h"

class Grid;
class Tile;

/*
 * @brief Uses Dijkstra's algorithm to determine reachable tiles and retraces paths to define movement and turn boundaries.
 */
class PathFindingComponent : public Components
{
public:
	PathFindingComponent(GameManager &gameManagerP);
	~PathFindingComponent();

	virtual void Update(float deltaTimeP) override;

	void ShowMovementGrid(const Vector3& startPositionP, int movementPointP, int tileTypeP = 1);
	void HideMovementGrid(bool isSelectedP = false);
	void UpdateGrid(int tileTypeP = 1);

	void RetracePath(const Tile &startTileP, Tile &targetTileP);
	const std::vector<TurnThreshold*>& GetTurnPath();

	std::vector<Vector3> lookPoints;
	int totalPathCost = 0;

	/* DEBUG */
	void DrawLines(const std::vector<TurnThreshold*>& turnBoudariesP);

private:
	int GetDistance(const Tile& tileAP, const Tile& tileBP);

	SceneManager &sceneManager;
	Grid &mGrid;

	std::vector<Tile*> mMovementGrid;
	std::unordered_map<Tile*, Tile*> mParentSet;
	std::vector<Vector3> mFinalPath;
	int mTurnDistance = 5;
	bool mIsGridVisisble = false;
};

