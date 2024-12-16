#pragma once

namespace Ogre 
{
    class SceneManager;
}

class GameManager;
class Grid;

class GameLevel
{
public:
	static void LoadEnvironment(Ogre::SceneManager &sceneManager, GameManager &gameEngine, Grid &mGrid);
	static void SetupBackgroundViewport(Ogre::SceneManager& sceneManagerP);
	static void SetupLights(Ogre::SceneManager& sceneManagerP);

	static void SpawnActors(GameManager& gameEngineP, Grid& gridP);
};

