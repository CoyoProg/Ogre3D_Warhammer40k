#pragma once
#include "Components.h"

class Grid;
class Tile;

class PathFindingComponent : public Components
{
public:
	PathFindingComponent(GameEngine& gameEngineP);
	~PathFindingComponent();

	virtual void Update(float deltaTime) override;

	bool FindPath(Vector3 startPositionP, Vector3 targetPositionP, int movementActionP);
	std::vector<Vector3> GetFinalPath() { return m_FinalPath; }

private:
	Grid& grid;

	std::map<Tile*, Tile*> m_ParentSet;
	std::vector<Vector3> m_FinalPath;

	int GetDistance(const Tile& tileA, const Tile& tileB);
	void RetracePath(Tile* startTile, Tile* targetTile);
};

