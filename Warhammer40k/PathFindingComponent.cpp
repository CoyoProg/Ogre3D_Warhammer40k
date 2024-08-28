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

void PathFindingComponent::GetMovementGrid(Vector3 startPositionP, int movementPointP, int tileType)
{
	m_MovementGrid.clear();
	m_ParentSet.clear();

	std::vector<Tile*> openSet;
	std::vector<Tile*> closeSet;

	Tile* startTile = grid.GetTile(startPositionP);
	startTile->gCost = 0;

	openSet.push_back(startTile);

	while (openSet.size() > 0)
	{
		Tile* currentTile = openSet[0];

		openSet.erase(openSet.begin());
		closeSet.emplace_back(currentTile);

		if (currentTile->gCost > movementPointP)
			continue;

		m_MovementGrid.push_back(currentTile);

		if (currentTile->GetType() != TILE_MOVEMENT_SELECTED)
			currentTile->SetTile(tileType);
		else if (tileType == TILE_MOVEMENT_ENEMY)
			currentTile->SetTile(TILE_MOVEMENT_BLEND);


		for (auto neighbours : grid.GetNeighboursTiles(currentTile))
		{
			bool isVisited = std::find(closeSet.begin(), closeSet.end(), neighbours) != closeSet.end();

			if (neighbours->GetType() == TILE_OBSTACLE || isVisited)
				continue;

			/* We Update the costs for each neighbours */
			int newMovementCost = currentTile->gCost + GetDistance(*currentTile, *neighbours);
			if (newMovementCost < neighbours->gCost)
			{
				neighbours->gCost = newMovementCost;

				m_ParentSet[neighbours] = currentTile;

				bool isOpened = std::find(openSet.begin(), openSet.end(), neighbours) != openSet.end();

				if (!isOpened)
				{
					openSet.push_back(neighbours);
				}
			}
		}
	}

	for (auto tiles : closeSet)
	{
		tiles->gCost = INT_MAX;
	}
}

void PathFindingComponent::HideMovementGrid(bool isSelectedP)
{
	if (!isSelectedP)
	{
		for (auto tiles : m_MovementGrid)
		{
			int tileType = tiles->GetType();

			if(tileType != TILE_MOVEMENT_SELECTED && tileType != TILE_MOVEMENT_BLEND)
				tiles->SetTile(TILE_EMPTY);
			else
				tiles->SetTile(TILE_MOVEMENT_SELECTED);
		}

		return;
	}

	for (auto tiles : m_MovementGrid)
		tiles->SetTile(TILE_EMPTY);

}

std::vector<TurnThreshold*> PathFindingComponent::GetTurnPath()
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
		if (dirToCurrentPoint == previousNormDir && i != lookPoints.size() - 1)
		{
			lookPoints.erase(lookPoints.begin() + i);
			finishLineIndex = lookPoints.size() - 1;
			i--;
			continue;
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
	m_FinalPath.clear();

	Tile* currentTile = targetTile;
	Vector3 currentTilePosition;

	m_StartPosition = grid.GetWorldPosition(startTile->gridCoordinates);
	m_TargetPosition = grid.GetWorldPosition(targetTile->gridCoordinates);

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
