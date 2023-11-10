#pragma once
#include "GameEngine.h"

class Grid
{
public:
	Grid(GameEngine& gameEngineP);
	~Grid();

private:
	void CreateGrid();
	GameEngine& m_gameEngine;
};

