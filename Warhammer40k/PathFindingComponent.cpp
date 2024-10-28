#include "PathFindingComponent.h"
#include "Grid.h"

#include <iostream>
#include <unordered_set>
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

void PathFindingComponent::ShowMovementGrid(const Vector3& startPositionP, int movementPointP, int tileTypeP)
{
	if (movementPointP < 10) return;

	/* Avoid redundant grid recalculations when transitioning a figurine from mouseOver to selected. 
	  We only update the tile colors directly. */
	if (mIsGridVisisble)
	{
		UpdateGrid();
		return;
	}

	mIsGridVisisble = true;

	/* The cost of moving to an adjacent tile is 10 for orthogonal movements and 14 for diagonal movements.
	Calculate the expected number of tiles that can be traversed based on available movement points. */
	int pathSizeReserve = (movementPointP / 10);
	int movementGridReserve = (pathSizeReserve * pathSizeReserve * 4);
	int openSetReserve = (pathSizeReserve * 2) * 4 - 4 ; // inner square perimeter of the movement grid

	mMovementGrid.clear();
	mParentSet.clear();

	std::vector<Tile*> openSet;
	std::unordered_set<Tile*> closedSet; // Use unordered_set for fast lookup

	mMovementGrid.reserve(movementGridReserve);
	mParentSet.reserve(movementGridReserve);
	closedSet.reserve(movementGridReserve);
	openSet.reserve(openSetReserve);

	Tile &startTile = *mGrid.GetTile(startPositionP);
	startTile.gCost = 0;

	openSet.emplace_back(&startTile);

	while (!openSet.empty())
	{
		Tile *currentTile = *openSet.begin();

		openSet.erase(openSet.begin());
		closedSet.emplace(currentTile);

		/* Check if movement cost exceeds limit */
		if (currentTile->gCost > movementPointP) continue;

		mMovementGrid.emplace_back(currentTile);

		/* Update tile type */
		if (currentTile->GetType() != TILE_MOVEMENT_SELECTED)
		{
			currentTile->SetTile(tileTypeP);
		}
		else if (tileTypeP == TILE_MOVEMENT_ENEMY)
		{
			currentTile->SetTile(TILE_MOVEMENT_BLEND);
		}

		/* Get neighboring  tiles */
		for (auto neighbour : mGrid.GetNeighboursTiles(*currentTile))
		{
			/* Skip obstacles and visited tiles */
			if (neighbour->GetType() == TILE_OBSTACLE || closedSet.find(neighbour) != closedSet.end()) continue;

			/* We Update the costs for each neighbour */
			int newMovementCost = currentTile->gCost + GetDistance(*currentTile, *neighbour);
			if (newMovementCost < neighbour->gCost)
			{
				neighbour->gCost = newMovementCost;

				mParentSet[neighbour] = currentTile;

				/* Check if the neighbour is already in the open set */
				if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
				{
					openSet.emplace_back(neighbour);
				}
			}
		}
	}

	for (auto tiles : closedSet)
	{
		tiles->gCost = INT_MAX;
	}
}

void PathFindingComponent::HideMovementGrid(bool isSelectedP)
{
	mIsGridVisisble = false;

	if (isSelectedP)
	{
		for (auto tiles : mMovementGrid)
		{
			tiles->SetTile(TILE_EMPTY);
		}

		return;
	}

	/* If not selected */
	for (auto tiles : mMovementGrid)
	{
		int mTileType = tiles->GetType();

		if (mTileType != TILE_MOVEMENT_SELECTED && mTileType != TILE_MOVEMENT_BLEND)
		{
			tiles->SetTile(TILE_EMPTY);
		}
		else
		{
			tiles->SetTile(TILE_MOVEMENT_SELECTED);
		}
	}
}

void PathFindingComponent::UpdateGrid(int tileTypeP)
{
	for (auto tiles : mMovementGrid)
	{
		if (tiles->GetType() != TILE_OBSTACLE)
		{
			tiles->SetTile(TILE_MOVEMENT_SELECTED);
		}
	}
}

const std::vector<TurnThreshold*>& PathFindingComponent::GetTurnPath()
{
	int expectedSize = mFinalPath.size();

	lookPoints.clear();

	std::vector<TurnThreshold*> turnBoundaries;
	turnBoundaries.reserve(expectedSize);
	lookPoints.reserve(expectedSize);

	lookPoints = mFinalPath;
	int lastTurnIndex = expectedSize - 1;

	Vector2 previousNormDirection{ 0,0 };

	for (int i = 1; i < lookPoints.size(); i++)
	{
		Vector2 previousPoint = Vector2(lookPoints[i - 1].x, lookPoints[i - 1].z);
		Vector2 currentPoint = Vector2(lookPoints[i].x, lookPoints[i].z);

		Vector2 nextNormDirection = (currentPoint - previousPoint);
		nextNormDirection.normalise();

		/* Remove duplicate consecutive normal directions by erasing the latest duplicate entry */
		if (nextNormDirection == previousNormDirection && i != lastTurnIndex)
		{
			lookPoints.erase(lookPoints.begin() + i);
			lastTurnIndex = lookPoints.size() - 1;			// Update last valid turn index
			i--;											// Adjust index to account for removed element
			continue;
		}

		previousNormDirection = nextNormDirection;

		/* Check if it's the last index */
		Vector2 turnBoundaryPoint = 
			(i == lastTurnIndex) ? 
			currentPoint : 
			currentPoint - nextNormDirection * mTurnDistance;

		turnBoundaries.emplace_back(new TurnThreshold(turnBoundaryPoint, previousPoint - nextNormDirection * mTurnDistance));
	}

	//DrawLines(turnBoundaries);

	return turnBoundaries;
}

void PathFindingComponent::DrawLines(const std::vector<TurnThreshold*>& turnBoudariesP)
{
	for (auto lines : turnBoudariesP)
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

void PathFindingComponent::RetracePath(const Tile& startTileP, Tile &targetTileP)
{
	mFinalPath.clear();
	mFinalPath.reserve(10);

	totalPathCost = 0;

	Tile *currentTile = &targetTileP;

	Vector3 startPosition = mGrid.GetWorldPosition(startTileP.gridCoordinates);
	Vector3 targetPosition = mGrid.GetWorldPosition(targetTileP.gridCoordinates);

	/* Traverse back from target to start, accumulating path cost and storing each point */
	while (currentTile != &startTileP)
	{
		totalPathCost += GetDistance(*currentTile, *mParentSet.at(currentTile));

		mFinalPath.insert(mFinalPath.begin(), mGrid.GetWorldPosition(currentTile->gridCoordinates));
		currentTile = mParentSet[currentTile];
	}
	startPosition.y = 0.0f;
	targetPosition.y = 0.0f;

	/* Add the start position at the beginning of the path */
	mFinalPath.insert(mFinalPath.begin(), startPosition);
}
