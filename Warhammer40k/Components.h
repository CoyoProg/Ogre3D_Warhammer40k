#pragma once
#include "GameManager.h"

/*
 * @brief Base class for all component types in the game.
 */
class Components
{
public:
	virtual void Update(float deltaTimeP) {};

private:
};