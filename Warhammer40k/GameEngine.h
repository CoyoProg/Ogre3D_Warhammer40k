#pragma once
#include "OgreApplicationContext.h"
#include "Ogre.h"

#include "GameManager.h"

using namespace Ogre;
using namespace OgreBites;

/*
 * @brief The GameEngine class manages the application context and input events,
 * handling frame rendering, key input, and initializing the renderer and game manager.
 */
class GameEngine : public ApplicationContext, public InputListener
{
public:
	// Begin OgreApplicationContextBase Interface
	virtual void setup() override;
	virtual bool frameRenderingQueued(const FrameEvent& fe) override;
	// End OgreApplicationContextBase Interface

	void InitializeRenderer();

private:
	// Begin InputListener Interface
	virtual bool keyPressed(const KeyboardEvent& evt) override;
	// End InputListener Interface

	GameManager mGameManager;
	SceneManager* mSceneManager = nullptr;
};

