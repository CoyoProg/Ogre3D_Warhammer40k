#include "Obstacles.h"
#include "GameEngine.h"
#include "Grid.h"
#include <iostream>

Obstacles::Obstacles(GameEngine& gameEngineP)
{
	SceneManager& sceneManager = *gameEngineP.GetSceneManager();
	Grid& grid = gameEngineP.GetGrid();

	/* Import Custom mesh */
	m_Entity = sceneManager.createEntity("WallEntity", "Wall.mesh");
	m_Entity->setCastShadows(false);
	m_Node = sceneManager.getRootSceneNode()->createChildSceneNode("WallNode");
	m_Node->setScale(10, 10, 10);
	m_Node->attachObject(m_Entity);

	// Set position
	m_Node->setPosition(grid.GetWorldCoords(Vector2(11, -5)));
	m_Node->yaw(Degree(-90));
	AddLineCollision(grid, m_Node->getPosition(), false, 5);

	/* Import Custom mesh */
	m_Entity = sceneManager.createEntity("WallEntity2", "Wall.mesh");
	m_Entity->setCastShadows(false);
	m_Node = sceneManager.getRootSceneNode()->createChildSceneNode("WallNode2");
	m_Node->setScale(10, 10, 10);
	m_Node->attachObject(m_Entity);

	// Set position
	m_Node->setPosition(grid.GetWorldCoords(Vector2(15, -7)));
	AddLineCollision(grid, m_Node->getPosition(), true, 5);
}

Obstacles::~Obstacles()
{
}

void Obstacles::AddLineCollision(Grid& gridP, Vector3 centerP, bool onAxisX, int lengthP)
{
	Vector2 originPosition = gridP.GetGridCoords(centerP);
	int newLength = lengthP / 2;

	if (onAxisX)
	{
		for (int i = -2; i < 3; i++)
		{
			gridP.SetTileMaterial(originPosition.x + i, originPosition.y );
		}
	}

	if (!onAxisX)
	{
		for (int i = -2; i < 3; i++)
		{
			gridP.SetTileMaterial(originPosition.x, originPosition.y + i);
		}
	}
}
