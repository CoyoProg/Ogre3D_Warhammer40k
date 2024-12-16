#ifdef _WIN32 
#include <windows.h>
#endif

#include "GameEngine.h"
#include "GameManager.h"

#include "OgreRTShaderSystem.h"
#include <OgreTrays.h>

void GameEngine::setup()
{
	InitializeRenderer();

	/* Setup the game manager */
	mGameManager.SetupGameManager(*ApplicationContext::getRenderWindow(), *mSceneManager, *this);
	mGameManager.LoadResources();
	mGameManager.InitializeGame();
}

bool GameEngine::frameRenderingQueued(const FrameEvent& fe)
{
	if (!mGameManager.isGameLoaded)
		mGameManager.isGameLoaded = true;

	/* Calculate the delta time (time since the last frame) */
	float deltaTime = fe.timeSinceLastFrame;

	/* Handle game logic and update here */
	mGameManager.Update(deltaTime);

	bool ret = ApplicationContext::frameRenderingQueued(fe);

	return ret;
}

void GameEngine::InitializeRenderer()
{
#ifdef _WIN32 
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	int screenWidth = desktop.right;
	int screenHeight = desktop.bottom;
#endif

	/* Create Custom Window */
	mRoot->initialise(false);
	createWindow("Warhammer 40k TableTop", screenWidth, screenHeight);

	/* Load ressources & initialise RTS */
	locateResources();
	initialiseRTShaderSystem();
	loadResources();

	/* Add frame & input listeners */
	mRoot->addFrameListener(this);
	addInputListener(this);

	/* Create scene manager */
	mSceneManager = mRoot->createSceneManager();

	/* Create overlay system */
	Ogre::OverlaySystem* overlaySystem = getOverlaySystem();
	mSceneManager->addRenderQueueListener(overlaySystem);

	/* register our scene with the RTSS */
	RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(mSceneManager);
}

bool GameEngine::keyPressed(const KeyboardEvent& evt)
{
	Keycode key = evt.keysym.sym;

	if (key == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}

	return true;
}