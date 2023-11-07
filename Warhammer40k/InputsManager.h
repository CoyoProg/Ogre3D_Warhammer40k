#pragma once

#include "Ogre.h"
#include "GameEngine.h"
#include "OgreApplicationContext.h"

using namespace Ogre;
using namespace OgreBites;

class InputsManager: public InputListener
{
public:
	InputsManager(GameEngine& gameManagerP) :
		m_gameManager(gameManagerP) {};

	virtual ~InputsManager() {};

	const std::set<Keycode>& getKeyPressed() const { return keysPressed; }
private:
	std::set<Keycode> keysPressed;

	GameEngine& m_gameManager;
	virtual bool keyPressed(const KeyboardEvent& evt) override;
	virtual bool keyReleased(const KeyboardEvent& evt) override;
};

