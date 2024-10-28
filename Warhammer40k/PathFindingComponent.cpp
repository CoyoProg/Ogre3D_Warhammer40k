#include "PathFindingComponent.h"
#include "Grid.h"

#include <iostream>
#include <map>

PathFindingComponent::PathFindingComponent(GameEngine& gameEngineP)  :
	mGrid(gameEngineP.GetGrid()),
	sceneManager(*gameEngineP.GetSceneManager())
{
}

PathFindingComponent::~PathFindingComponent()
{
}

void PathFindingComponent::Update(float deltaTimeP)
{
}

void PathFindingComponent::GetMovementGrid(const Vector3& startPositionP, int movementPointP, int tileTypeP)
{
	mMovementGrid.clear();
	mParentSet.clear();

	std::vector<Tile*> openSet;
	std::vector<Tile*> closeSet;

	Tile* startTile = mGrid.GetTile(startPositionP);
	startTile->gCost = 0;

	openSet.push_back(startTile);

	while (openSet.size() > 0)
	{
		Tile* currentTile = openSet[0];

		openSet.erase(openSet.begin());
		closeSet.emplace_back(currentTile);

		if (currentTile->gCost > movementPointP)
			continue;

		mMovementGrid.push_back(currentTile);

		if (currentTile->GetType() != TILE_MOVEMENT_SELECTED)
			currentTile->SetTile(tileTypeP);
		else if (tileTypeP == TILE_MOVEMENT_ENEMY)
			currentTile->SetTile(TILE_MOVEMENT_BLEND);


		for (auto neighbours : mGrid.GetNeighboursTiles(currentTile))
		{
			bool isVisited = std::find(closeSet.begin(), closeSet.end(), neighbours) != closeSet.end();

			if (neighbours->GetType() == TILE_OBSTACLE || isVisited)
				continue;

			/* We Update the costs for each neighbours */
			int newMovementCost = currentTile->gCost + GetDistance(*currentTile, *neighbours);
			if (newMovementCost < neighbours->gCost)
			{
				neighbours->gCost = newMovementCost;

				mParentSet[neighbours] = currentTile;

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
		for (auto tiles : mMovementGrid)
		{
			int mTileType = tiles->GetType();

			if(mTileType != TILE_MOVEMENT_SELECTED && mTileType != TILE_MOVEMENT_BLEND)
				tiles->SetTile(TILE_EMPTY);
			else
				tiles->SetTile(TILE_MOVEMENT_SELECTED);
		}

		return;
	}

	for (auto tiles : mMovementGrid)
		tiles->SetTile(TILE_EMPTY);
}

std::vector<TurnThreshold*> PathFindingComponent::GetTurnPath()
{
	lookPoints.clear();
	turnBoundaries.clear();

	lookPoints = mFinalPath;
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

void PathFindingComponent::DrawLines()
{
	for (auto lines : turnBoundaries)
	{
		lines->DrawLine(&sceneManager);
	}
}

int PathFindingComponent::GetDistance(const Tile& tileAP, const Tile& tileBP)
{
	int distanceX = abs(tileAP.gridCoordinates.x - tileBP.gridCoordinates.x);
	int distanceZ = abs(tileAP.gridCoordinates.y - tileBP.gridCoordinates.y);

	if (distanceX > distanceZ)
		return 14 * distanceZ + 10 * (distanceX - distanceZ);

	return 14 * distanceX + 10 * (distanceZ - distanceX);
}

void PathFindingComponent::RetracePath(Tile *startTileP, Tile *targetTileP)
{
	mFinalPath.clear();
	totalPathCost = 0;

	Tile *currentTile = targetTileP;
	Vector3 currentTilePosition;

	Vector3 startPosition = mGrid.GetWorldPosition(startTileP->gridCoordinates);
	Vector3 targetPosition = mGrid.GetWorldPosition(targetTileP->gridCoordinates);

	while (currentTile != startTileP)
	{
		totalPathCost += GetDistance(*currentTile, *mParentSet[currentTile]);

		currentTilePosition = mGrid.GetWorldPosition(currentTile->gridCoordinates);
		mFinalPath.insert(mFinalPath.begin(), currentTilePosition);
		currentTile = mParentSet[currentTile];
	}
	startPosition.y = 0.0f;
	targetPosition.y = 0.0f;

	mFinalPath.insert(mFinalPath.begin(), startPosition);
	mFinalPath.back() = targetPosition;
}
