#pragma once
#include "Actors.h"

class TableTop : public Actors
{
public:
	TableTop(GameEngine& gameEngineP);
	~TableTop() {};

	SceneNode* GetSceneNode() { return tabletopNode; }
private:
	SceneNode* tabletopNode;
};

