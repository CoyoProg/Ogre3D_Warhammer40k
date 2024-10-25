#pragma once
#include "Actors.h"

/*
 * @brief Represents the game’s tabletop area, acting as the main surface for game elements.
 */
class TableTop : public Actors
{
public:
	TableTop(GameEngine &gameEngineP);
	~TableTop() {};

	SceneNode* GetSceneNode() { return mTabletopNode; }

private:
	SceneNode *mTabletopNode = nullptr;
};

