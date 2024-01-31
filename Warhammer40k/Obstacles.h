#pragma once
#include "Actors.h"

class GameEngine;

class Obstacles : public Actors
{
public:
	Obstacles(GameEngine& gameEngineP, Vector3 positionP, std::string ID, bool flip = false);
	~Obstacles();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="centerP"> Center of the collision Box</param>
	/// <param name="onAxisX">: If the collision Box is on the X axis (true) or Z axis (false)</param>
	/// <param name="lengthP">: How many nodes occupy the collision Box </param>
	void AddLineCollision(Grid& gridP, Vector3 centerP, bool onAxisX, int lengthP);

	bool isFlipped = false;
};

