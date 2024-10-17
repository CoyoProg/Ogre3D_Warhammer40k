#pragma once
#include "Components.h"

class Figurines;

class CameraComponent : public Components, public InputListener
{
public:
	CameraComponent(GameEngine& gameEngineP);
	~CameraComponent();

	virtual void Update(float deltaTimeP) override;
	Camera* getCamera() { return mCamera; }

private:
	/* Camera properties */
	float mCameraSpeed = 50.f;
	float mMoveX = 0.f;
	float mMoveZ = 0.f;
	float mZoomSpeed = 20.f;
	float mClampMinY = 20.f;
	float mClampMaxY = 175.f;
	float mClampMaxX = 300.f;
	float mClampMinZ = -55.f;
	float mClampMaxZ = 300.f;
	Camera *mCamera = nullptr;
	SceneNode *mCamNode = nullptr;

	/* Inputs Listener*/
	virtual bool mouseWheelRolled(const MouseWheelEvent& evt) override;
	virtual bool keyPressed(const KeyboardEvent& evt) override;
	virtual bool keyReleased(const KeyboardEvent& evt) override;
	virtual void frameRendered(const FrameEvent& evt) override;

	int mMouseWheelY = 0;
	void UpdateCameraPosition(float deltaSec);
	void Zoom(float deltaTime);
};

