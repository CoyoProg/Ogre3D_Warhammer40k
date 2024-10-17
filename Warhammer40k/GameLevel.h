#pragma once

namespace Ogre 
{
    class SceneManager;
}

class GameEngine;
class Grid;

class GameLevel
{
public:
	static void LoadLevel(GameEngine &gameEngineP);

private:
	static void LoadEnvironment(Ogre::SceneManager &sceneManager, GameEngine &gameEngine, Grid *mGrid);
};

