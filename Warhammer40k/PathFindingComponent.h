#pragma once
#include "Components.h"
#include "TurnThreshold.h"

class Grid;
class Tile;

class PathFindingComponent : public Components
{
public:
	PathFindingComponent(GameEngine& gameEngineP);
	~PathFindingComponent();

	virtual void Update(float deltaTime) override;

	bool FindPath(Vector3 startPositionP, Vector3 targetPositionP, int movementActionP);
	std::vector<TurnThreshold*> GetTurnPath();

	std::vector<Vector3> lookPoints;
	std::vector<TurnThreshold*> turnBoundaries;
	int finishLineIndex;
	int turnDst = 5;
	SceneManager* sceneManager;

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
	Grid& grid;

	std::map<Tile*, Tile*> m_ParentSet;
	std::vector<Vector3> m_FinalPath;
	Vector3 m_StartPosition;
	Vector3 m_TargetPosition;

	int GetDistance(const Tile& tileA, const Tile& tileB);
	void RetracePath(Tile* startTile, Tile* targetTile);
};

