#include "PathFindingComponent.h"
#include "Grid.h"
#include <iostream>
#include <map>

PathFindingComponent::PathFindingComponent(GameEngine& gameEngineP)  :
	grid(gameEngineP.GetGrid()),
	sceneManager(gameEngineP.GetSceneManager()),
	finishLineIndex(0)
{
}

PathFindingComponent::~PathFindingComponent()
{
}

void PathFindingComponent::Update(float deltaTime)
{
}

bool PathFindingComponent::FindPath(Vector3 startPositionP, Vector3 targetPositionP, int movementActionP)
{
	m_StartPosition = startPositionP;
	m_TargetPosition = targetPositionP;
	m_ParentSet.clear();
	m_FinalPath.clear();

	Tile* startTile = grid.GetTile(m_StartPosition);
	Tile* targetTile = grid.GetTile(m_TargetPosition);

	if (targetTile->GetType() == TILE_OBSTACLE)
		return false;

	std::vector<Tile*> openSet;
	std::vector<Tile*> closeSet;

	openSet.push_back(startTile);
	int currentIndex = 0;

	while (openSet.size() > 0)
	{
		Tile* currentTile = openSet[currentIndex];

		/* We check the lowest F cost of the neighbours Tiles */
		for (int i = 1; i < openSet.size(); i++)
		{
			if (openSet[i]->GetFCost() < currentTile->GetFCost() || 
				openSet[i]->GetFCost() == currentTile->GetFCost() && openSet[i]->hCost < currentTile->hCost)
			{
				currentTile = openSet[i];
				currentIndex = i;
			}
		}

		/* We Close the lowest cost */
		openSet.erase(openSet.begin() + currentIndex);
		closeSet.emplace_back(currentTile);

		/* If the lowest cost was the targetTile then the algorithm is done */
		if (currentTile == targetTile)
		{
			totalCost = targetTile->gCost;

			for (auto tiles : openSet)
			{
				tiles->gCost = 0;
				tiles->hCost = 0;
			}

			for (auto tiles : closeSet)
			{
				tiles->gCost = 0;
				tiles->hCost = 0;
			}

			if (totalCost / GRID_MULTIPLICATEUR > movementActionP)
				return false;

			RetracePath(startTile, targetTile);

			totalCost = totalCost / GRID_MULTIPLICATEUR;

			return true;
		}

		/* We add the new neighbours to openSet */
		for (auto neighbours : grid.GetNeighboursTiles(currentTile))
		{
			bool isVisited = std::find(closeSet.begin(), closeSet.end(), neighbours) != closeSet.end();

			if (neighbours->GetType() == TILE_OBSTACLE || isVisited)
				continue;

			/* We Update the costs (hCost & gCost) for each neighbours */
			bool isOpened = std::find(openSet.begin(), openSet.end(), neighbours) != openSet.end();

			int newMovementCost = currentTile->gCost + GetDistance(*currentTile, *neighbours);
			if (newMovementCost < neighbours->gCost || !isOpened)
			{
				neighbours->gCost = newMovementCost;
				neighbours->hCost = GetDistance(*neighbours, *targetTile);

				m_ParentSet[neighbours] = currentTile;

				if (!isOpened)
					openSet.push_back(neighbours);
			}
		}
	}
}

std::vector<TurnThreshold*>  PathFindingComponent::GetTurnPath()
{
	lookPoints.clear();
	turnBoundaries.clear();

	lookPoints = m_FinalPath;
	finishLineIndex = lookPoints.size() - 1;

	Vector2 previousNormDir{ 0,0 };

	for (int i = 1; i < lookPoints.size(); i++)
	{
		Vector2 previousPoint = Vector2(lookPoints[i - 1].x, lookPoints[i - 1].z);

		Vector2 currentPoint = Vector2(lookPoints[i].x, lookPoints[i].z);
		Vector2 dirToCurrentPoint = (currentPoint - previousPoint);
		dirToCurrentPoint.normalise();
		if (dirToCurrentPoint == previousNormDir)
		{
			//lookPoints.erase(lookPoints.begin() + i);
			//finishLineIndex = lookPoints.size() - 1;
			//i--;
			//continue;
		}

		previousNormDir = dirToCurrentPoint;

		Vector2 turnBoundaryPoint = (i == finishLineIndex) ?
			currentPoint : currentPoint - dirToCurrentPoint * turnDst;

		turnBoundaries.emplace_back(new TurnThreshold(turnBoundaryPoint, previousPoint - dirToCurrentPoint * turnDst));
	}

	//DrawLines();

	return turnBoundaries;
}

int PathFindingComponent::GetDistance(const Tile& tileA, const Tile& tileB)
{
	int distanceX = abs(tileA.gridCoordinates.x - tileB.gridCoordinates.x);
	int distanceZ = abs(tileA.gridCoordinates.y - tileB.gridCoordinates.y);

	if (distanceX > distanceZ)
		return 14 * distanceZ + 10 * (distanceX - distanceZ);

	return 14 * distanceX + 10 * (distanceZ - distanceX);
}

void PathFindingComponent::RetracePath(Tile* startTile, Tile* targetTile)
{
	Tile* currentTile = targetTile;
	Vector3 currentTilePosition;

	while (currentTile != startTile)
	{
		currentTilePosition = grid.GetWorldPosition(currentTile->gridCoordinates);
		m_FinalPath.insert(m_FinalPath.begin(), currentTilePosition);
		currentTile = m_ParentSet[currentTile];
	}
	m_StartPosition.y = 0.0f;
	m_TargetPosition.y = 0.0f;

	m_FinalPath.insert(m_FinalPath.begin(), m_StartPosition);
	m_FinalPath.back() = m_TargetPosition;
}
