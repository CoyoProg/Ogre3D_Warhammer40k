#include "Player.h"
#include "CameraComponent.h"

Player::Player(GameEngine& gameEngineP) :
	m_GameEngine (gameEngineP)
{
	CameraComponent* camera = new CameraComponent(gameEngineP);
	gameEngineP.getRenderWindow()->addViewport(camera->getCamera());
	AddComponent(camera);
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	Actors::Update(deltaTime);
}
