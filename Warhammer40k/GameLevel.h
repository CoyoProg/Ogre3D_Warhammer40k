#pragma once

namespace Ogre 
{
    class SceneManager;
}

class GameEngine;

class GameLevel
{
public:
	static void LoadLevel(GameEngine& gameEngineP);
private:
	static void LoadTuto(Ogre::SceneManager& sceneManagerP);
};

