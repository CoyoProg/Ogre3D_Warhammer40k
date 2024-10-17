#include "Obstacles.h"
#include "GameEngine.h"
#include "Grid.h"
#include <iostream>
#include "QueryFlags.h"

Obstacles::Obstacles(GameEngine &gameEngineP, Vector2 gridCoordsP, Vector3 scaleP, std::string ID):
	mGridCoords(gridCoordsP),
	mScale(scaleP)
{
	/* REFERENCES */
	SceneManager &sceneManager = *gameEngineP.GetSceneManager();
	Grid &mGrid = gameEngineP.GetGrid();


	/* CREATE CUBE OBJECT */
	mManualObject = sceneManager.createManualObject("ObstacleObject" + ID);

	mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mManualObject->setCastShadows(false);


	/* VERTEX */
	// Front face
	mManualObject->position(-0.5, -0.5, 0.5); // Vertex 0
	mManualObject->normal(0, 0, 1);
	mManualObject->textureCoord(0, 0);

	mManualObject->position(0.5, -0.5, 0.5);  // Vertex 1
	mManualObject->normal(0, 0, 1);
	mManualObject->textureCoord(1, 0);

	mManualObject->position(0.5, 0.5, 0.5);  // Vertex 2
	mManualObject->normal(0, 0, 1);
	mManualObject->textureCoord(1, 1);

	mManualObject->position(-0.5, 0.5, 0.5); // Vertex 3
	mManualObject->normal(0, 0, 1);
	mManualObject->textureCoord(0, 1);

	// Back face
	mManualObject->position(-0.5, -0.5, -0.5); // Vertex 4
	mManualObject->normal(0, 0, -1);
	mManualObject->textureCoord(1, 0);

	mManualObject->position(0.5, -0.5, -0.5);  // Vertex 5
	mManualObject->normal(0, 0, -1);
	mManualObject->textureCoord(0, 0);

	mManualObject->position(0.5, 0.5, -0.5);  // Vertex 6
	mManualObject->normal(0, 0, -1);
	mManualObject->textureCoord(0, 1);

	mManualObject->position(-0.5, 0.5, -0.5); // Vertex 7
	mManualObject->normal(0, 0, -1);
	mManualObject->textureCoord(1, 1);

	/* INDICES */
	// Front face
	mManualObject->index(0); mManualObject->index(1); mManualObject->index(2);
	mManualObject->index(2); mManualObject->index(3); mManualObject->index(0);

	// Back face
	mManualObject->index(6); mManualObject->index(5); mManualObject->index(4);
	mManualObject->index(4); mManualObject->index(7); mManualObject->index(6);

	// Top face
	mManualObject->index(3); mManualObject->index(2); mManualObject->index(6);
	mManualObject->index(6); mManualObject->index(7); mManualObject->index(3);

	// Bottom face
	mManualObject->index(1); mManualObject->index(0); mManualObject->index(4);
	mManualObject->index(4); mManualObject->index(5); mManualObject->index(1);

	// Left face
	mManualObject->index(0); mManualObject->index(3); mManualObject->index(7);
	mManualObject->index(7); mManualObject->index(4); mManualObject->index(0);

	// Right face
	mManualObject->index(2); mManualObject->index(1); mManualObject->index(5);
	mManualObject->index(5); mManualObject->index(6); mManualObject->index(2);

	mManualObject->end();
	mManualObject->convertToMesh("ObstacleMesh" + ID);

	mNode = sceneManager.getRootSceneNode()->createChildSceneNode("ObstacleNode" + ID);
	mNode->setScale(mScale * GRID_CELL_SIZE);
	mNode->attachObject(mManualObject);
	mManualObject->setQueryFlags((QueryFlags::OBSTACLE_MASK));



	/* POSITION */
	Vector3 offsetPosition(0);
	offsetPosition.y = mScale.y / 2;

	if (mScale.x > 1)
	{
		offsetPosition.x = (mScale.x / 2) * GRID_CELL_SIZE - GRID_CELL_SIZE / 2;
	}

	if (mScale.z > 1)
	{
		offsetPosition.z = -(mScale.z / 2) * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
	}

	Vector3 newPosition = mGrid.GetWorldPosition(mGridCoords);
	mNode->setPosition(newPosition + offsetPosition);

	//mManualObject->setVisible(false);

	UpdateCollisions(mGrid);
}

Obstacles::~Obstacles()
{
}

void Obstacles::UpdateCollisions(Grid &gridP, bool onFlipP)
{
	for (int x = -1; x < mScale.x + 1; x++)
	{
		for (int z = -1; z < mScale.z + 1; z++)
		{
			gridP.SetTileMaterial(mGridCoords.x + x, -mGridCoords.y - z, TILE_OBSTACLE);
		}
	}
}

void Obstacles::FlipCollisions(Grid &gridP)
{
	mOnFlip = !mOnFlip;

	int flipMult = 1;
	int newCoordX = mGridCoords.x;
	int newCoordZ = mGridCoords.y;

	if (mOnFlip)
	{
		newCoordX = (GRID_SIZE_X -1) - mGridCoords.x;
		newCoordZ = (GRID_SIZE_Z - 1) - mGridCoords.y;

		flipMult = -1;
	}

	for (int x = -1; x < mScale.x + 1; x++)
	{
		for (int z = -1; z < mScale.z + 1; z++)
		{
			gridP.SetTileMaterial(newCoordX + x * flipMult, -newCoordZ - z * flipMult, TILE_OBSTACLE);
		}
	}
}
