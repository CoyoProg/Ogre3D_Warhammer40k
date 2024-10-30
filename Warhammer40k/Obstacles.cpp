#include "Obstacles.h"
#include "QueryFlags.h"

#include "Grid.h"

constexpr float GRID_OFFSET_CENTER = GRID_CELL_SIZE / 2.0f;

Obstacles::Obstacles(GameManager &gameManagerP, const Vector2 &gridCoordsP, const Vector3 &scaleP, const std::string &ID):
	mGridCoords(gridCoordsP),
	mScale(scaleP),
	mGrid(gameManagerP.GetGrid()),
	mSceneManager(gameManagerP.GetSceneManager())
{
	CreateCube(ID);

	mNode = mSceneManager.getRootSceneNode()->createChildSceneNode("ObstacleNode" + ID);
	mNode->setScale(mScale * GRID_CELL_SIZE);
	mNode->attachObject(mManualObject);
	mManualObject->setQueryFlags((QueryFlags::OBSTACLE_MASK));

	SetCubePosition();

	UpdateTilesCollision(mGridCoords.x, mGridCoords.y, false);
}

void Obstacles::SetCubePosition()
{
	Vector3 offsetPosition(0);
	offsetPosition.y = mScale.y * 0.5f;

	if (mScale.x > 1)
	{
		offsetPosition.x = (mScale.x * GRID_CELL_SIZE) * 0.5f - GRID_OFFSET_CENTER;
	}

	if (mScale.z > 1)
	{
		offsetPosition.z = -(mScale.z * GRID_CELL_SIZE) * 0.5f + GRID_OFFSET_CENTER;
	}

	Vector3 newPosition = mGrid.GetWorldPosition(mGridCoords);
	mNode->setPosition(newPosition + offsetPosition);
}

void Obstacles::CreateCube(const std::string& ID)
{
	/* Create the cube object */
	mManualObject = mSceneManager.createManualObject("ObstacleObject" + ID);

	mManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mManualObject->setCastShadows(true);

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
	const int faces[6][6] = {
		{ 0, 1, 2, 2, 3, 0 }, // Front face
		{ 6, 5, 4, 4, 7, 6 }, // Back face
		{ 3, 2, 6, 6, 7, 3 }, // Top face
		{ 1, 0, 4, 4, 5, 1 }, // Bottom face
		{ 0, 3, 7, 7, 4, 0 }, // Left face
		{ 2, 1, 5, 5, 6, 2 }  // Right face
	};

	// Define indices
	for (const auto& face : faces) 
	{
		for (int index : face) 
		{
			mManualObject->index(index);
		}
	}

	mManualObject->end();
	mManualObject->convertToMesh("ObstacleMesh" + ID);
}

Obstacles::~Obstacles()
{
}

void Obstacles::UpdateTilesCollision(int coordXP, int coordYP, bool isFlippedP)
{
	int flipMult = isFlippedP ? -1 : 1;

	for (int x = -1; x < mScale.x + 1; x++)
	{
		for (int z = -1; z < mScale.z + 1; z++)
		{
			mGrid.SetTileCollision(coordXP + x * flipMult, -coordYP - z * flipMult, TILE_OBSTACLE);
		}
	}
}

void Obstacles::FlipCollisions()
{
	mIsFlipped = !mIsFlipped;

	int newCoordX = mGridCoords.x;
	int newCoordZ = mGridCoords.y;

	if (mIsFlipped)
	{
		newCoordX = (GRID_SIZE_X -1) - mGridCoords.x;
		newCoordZ = (GRID_SIZE_Z - 1) - mGridCoords.y;
	}

	UpdateTilesCollision(newCoordX, newCoordZ, mIsFlipped);
}
