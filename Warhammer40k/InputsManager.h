#pragma once
#include "GameEngine.h"
#include "IListenerInputs.h"

using namespace Ogre;
using namespace OgreBites;

class InputsManager : public InputListener
{
public:
	InputsManager(GameEngine& gameManagerP) :
		m_GameManager(gameManagerP) {};

	virtual ~InputsManager() {};

	const int getMouseWheelY() const { return m_MouseWheelY; }
	void AddListener(IListenerInputs* listenerP) { m_MyListeners.emplace_back(listenerP); }

private:
	int m_MouseWheelY{ 0 };
	bool m_IsLMBDown{ false };
	bool m_IsRMBDown{ false };

	GameEngine& m_GameManager;
	std::vector<IListenerInputs*> m_MyListeners;

	virtual bool keyPressed(const KeyboardEvent& evt) override;
	virtual bool keyReleased(const KeyboardEvent& evt) override;
	virtual bool mouseWheelRolled(const MouseWheelEvent& evt) override;
	virtual bool mousePressed(const MouseButtonEvent& evt) override;
	virtual bool mouseReleased(const MouseButtonEvent& evt) override;

	virtual void frameRendered(const FrameEvent& evt) override;
};

