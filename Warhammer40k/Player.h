#pragma once
#include "Actors.h"

class Components;

class Player : public Actors
{
public:
	Player(GameEngine& gameEngineP);
	~Player();

	virtual void Update(float deltaTime) override;

private:
	GameEngine& m_GameEngine;
};

