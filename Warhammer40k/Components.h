#pragma once
#include "GameEngine.h"

/*
 * @brief Base class for all component types in the game.
 */
class Components
{
public:
	virtual void Update(float deltaTimeP) {};

private:
};