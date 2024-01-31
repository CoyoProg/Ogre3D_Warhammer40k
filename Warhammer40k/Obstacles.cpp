#include "Obstacles.h"
#include "GameEngine.h"
#include "Grid.h"
#include <iostream>
#include "QueryFlags.h"

Obstacles::Obstacles(GameEngine& gameEngineP, Vector3 positionP, std::string ID, bool flip)
{
	SceneManager& sceneManager = *gameEngineP.GetSceneManager();
	Grid& grid = gameEngineP.GetGrid();

	/* Import Custom mesh */
	m_Entity = sceneManager.createEntity("WallEntity " + ID, "Wall.mesh");
	m_Entity->setCastShadows(false);
	//m_Entity->setMaterialName("ruinsTexture");
	m_Node = sceneManager.getRootSceneNode()->createChildSceneNode("WallNode " + ID);
	m_Node->setScale(.1, .1, .2);
	m_Node->attachObject(m_Entity);
	m_Entity->setQueryFlags(QueryFlags::OBSTACLE_MASK);

	// Set position
	m_Node->setPosition(positionP);

	if(flip)
		m_Node->yaw(Degree(-90));

	isFlipped = flip;

	AddLineCollision(grid, m_Node->getPosition(), !flip, 2 * GRID_MULTIPLICATEUR);
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
		for (int i = -newLength; i < (newLength + 1); i++)
		{
			gridP.SetTileMaterial(originPosition.x + i, originPosition.y, TILE_OBSTACLE);
		}
	}

	if (!onAxisX)
	{
		for (int i = -newLength; i < (newLength +1); i++)
		{
			gridP.SetTileMaterial(originPosition.x, originPosition.y + i, TILE_OBSTACLE);
		}
	}
}
