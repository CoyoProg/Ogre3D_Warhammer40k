#include "Obstacles.h"
#include "GameEngine.h"
#include "Grid.h"
#include <iostream>
#include "QueryFlags.h"

Obstacles::Obstacles(GameEngine& gameEngineP, Vector2 gridCoordsP, Vector3 scaleP, std::string ID):
	m_GridCoords(gridCoordsP),
	m_Scale(scaleP)
{
	/* REFERENCES */
	SceneManager& sceneManager = *gameEngineP.GetSceneManager();
	Grid& grid = gameEngineP.GetGrid();


	/* CREATE CUBE OBJECT */
	m_ManualObject = sceneManager.createManualObject("ObstacleObject" + ID);

	m_ManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	m_ManualObject->setCastShadows(false);


	/* VERTEX */
	// Front face
	m_ManualObject->position(-0.5, -0.5, 0.5); // Vertex 0
	m_ManualObject->normal(0, 0, 1);
	m_ManualObject->textureCoord(0, 0);

	m_ManualObject->position(0.5, -0.5, 0.5);  // Vertex 1
	m_ManualObject->normal(0, 0, 1);
	m_ManualObject->textureCoord(1, 0);

	m_ManualObject->position(0.5, 0.5, 0.5);  // Vertex 2
	m_ManualObject->normal(0, 0, 1);
	m_ManualObject->textureCoord(1, 1);

	m_ManualObject->position(-0.5, 0.5, 0.5); // Vertex 3
	m_ManualObject->normal(0, 0, 1);
	m_ManualObject->textureCoord(0, 1);

	// Back face
	m_ManualObject->position(-0.5, -0.5, -0.5); // Vertex 4
	m_ManualObject->normal(0, 0, -1);
	m_ManualObject->textureCoord(1, 0);

	m_ManualObject->position(0.5, -0.5, -0.5);  // Vertex 5
	m_ManualObject->normal(0, 0, -1);
	m_ManualObject->textureCoord(0, 0);

	m_ManualObject->position(0.5, 0.5, -0.5);  // Vertex 6
	m_ManualObject->normal(0, 0, -1);
	m_ManualObject->textureCoord(0, 1);

	m_ManualObject->position(-0.5, 0.5, -0.5); // Vertex 7
	m_ManualObject->normal(0, 0, -1);
	m_ManualObject->textureCoord(1, 1);

	/* INDICES */
	// Front face
	m_ManualObject->index(0); m_ManualObject->index(1); m_ManualObject->index(2);
	m_ManualObject->index(2); m_ManualObject->index(3); m_ManualObject->index(0);

	// Back face
	m_ManualObject->index(6); m_ManualObject->index(5); m_ManualObject->index(4);
	m_ManualObject->index(4); m_ManualObject->index(7); m_ManualObject->index(6);

	// Top face
	m_ManualObject->index(3); m_ManualObject->index(2); m_ManualObject->index(6);
	m_ManualObject->index(6); m_ManualObject->index(7); m_ManualObject->index(3);

	// Bottom face
	m_ManualObject->index(1); m_ManualObject->index(0); m_ManualObject->index(4);
	m_ManualObject->index(4); m_ManualObject->index(5); m_ManualObject->index(1);

	// Left face
	m_ManualObject->index(0); m_ManualObject->index(3); m_ManualObject->index(7);
	m_ManualObject->index(7); m_ManualObject->index(4); m_ManualObject->index(0);

	// Right face
	m_ManualObject->index(2); m_ManualObject->index(1); m_ManualObject->index(5);
	m_ManualObject->index(5); m_ManualObject->index(6); m_ManualObject->index(2);

	m_ManualObject->end();
	m_ManualObject->convertToMesh("ObstacleMesh" + ID);

	m_Node = sceneManager.getRootSceneNode()->createChildSceneNode("ObstacleNode" + ID);
	m_Node->setScale(m_Scale * GRID_CELL_SIZE);
	m_Node->attachObject(m_ManualObject);
	m_ManualObject->setQueryFlags((QueryFlags::OBSTACLE_MASK));



	/* POSITION */
	Vector3 offsetPosition(0);
	offsetPosition.y = m_Scale.y / 2;

	if (m_Scale.x > 1)
	{
		offsetPosition.x = (m_Scale.x / 2) * GRID_CELL_SIZE - GRID_CELL_SIZE / 2;
	}

	if (m_Scale.z > 1)
	{
		offsetPosition.z = -(m_Scale.z / 2) * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
	}

	Vector3 newPosition = grid.GetWorldPosition(m_GridCoords);
	m_Node->setPosition(newPosition + offsetPosition);

	//m_ManualObject->setVisible(false);

	UpdateCollisions(grid);
}

Obstacles::~Obstacles()
{
}

void Obstacles::UpdateCollisions(Grid& gridP, bool OnFlip)
{
	for (int x = -1; x < m_Scale.x + 1; x++)
	{
		for (int z = -1; z < m_Scale.z + 1; z++)
		{
			gridP.SetTileMaterial(m_GridCoords.x + x, -m_GridCoords.y - z, TILE_OBSTACLE);
		}
	}
}

void Obstacles::FlipCollisions(Grid& gridP)
{
	m_OnFlip = !m_OnFlip;

	int flipMult = 1;
	int newCoordX = m_GridCoords.x;
	int newCoordZ = m_GridCoords.y;

	if (m_OnFlip)
	{
		newCoordX = (GRID_SIZE_X -1) - m_GridCoords.x;
		newCoordZ = (GRID_SIZE_Z - 1) - m_GridCoords.y;

		flipMult = -1;
	}

	for (int x = -1; x < m_Scale.x + 1; x++)
	{
		for (int z = -1; z < m_Scale.z + 1; z++)
		{
			gridP.SetTileMaterial(newCoordX + x * flipMult, -newCoordZ - z * flipMult, TILE_OBSTACLE);
		}
	}
}
