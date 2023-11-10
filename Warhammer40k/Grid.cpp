#include "Grid.h"


Grid::Grid(GameEngine& gameEngineP) :
	m_gameEngine(gameEngineP)
{
	CreateGrid();
}

Grid::~Grid()
{

}

void Grid::CreateGrid()
{
	SceneManager& sceneManager = *m_gameEngine.GetSceneManager();
	////// GRID /////
	int gridSizeZ = 28;
	int gridSizeX = 18;
	int gridCellSize = 10;
	int padding = 1.f;

	const Vector3 offset(-90, 0, 250);

	int count = 0;
	for (int i = 0; i < gridSizeX; i++)
	{
		for (int j = 0; j < gridSizeZ; j++)
		{
			count++;
			std::string entityName = "Plane " + std::to_string(count);
			std::string nodeName = "Node " + std::to_string(count);

			Entity* planeEntity = sceneManager.createEntity(entityName, SceneManager::PT_PLANE);
			planeEntity->setMaterialName("Tile_Valid");
			SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
			planeNode->attachObject(planeEntity);
			planeNode->setPosition(Vector3(i * gridCellSize + gridCellSize / 2, 0.2f, -j * gridCellSize - gridCellSize / 2) + offset);
			planeNode->pitch(Degree(-90));
			planeNode->setScale(Vector3(.045f, .045f, 1.f));
		}
	}

	Entity* planeEntity = sceneManager.createEntity("BackgroundTile", SceneManager::PT_PLANE);
	planeEntity->setMaterialName("Tile_Background");
	SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	planeNode->attachObject(planeEntity);
	planeNode->setPosition(Vector3(0, 0.1f, 105));
	planeNode->pitch(Degree(-90));
	planeNode->setScale(Vector3(0.95f, 1.45f, 1.f));
}
