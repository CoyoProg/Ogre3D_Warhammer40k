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
	static void LoadLevel(GameManager &gameEngineP);

private:
	static void LoadEnvironment(Ogre::SceneManager &sceneManager, GameManager &gameEngine, Grid *mGrid);
};

