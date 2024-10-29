#pragma once
#include "Components.h"

class Figurines;

/*
 * @brief Manages the camera's movement, zoom, and input controls within the game world.
 * This component allows the camera to respond to keyboard and mouse inputs for
 * panning, zooming, and boundary clamping.
 */
class CameraComponent : public Components, public InputListener
{
public:
	CameraComponent(GameManager& gameManagerP);
	~CameraComponent();

	virtual void Update(float deltaTimeP) override;
	Camera& getCamera() const { return *mCamera; }

private:
	void UpdateCameraPosition(float deltaSec);
	void Zoom(float deltaTime);

	virtual bool mouseWheelRolled(const MouseWheelEvent& evt) override;
	virtual bool keyPressed(const KeyboardEvent& evt) override;
	virtual bool keyReleased(const KeyboardEvent& evt) override;

	Camera* mCamera = nullptr;
	SceneNode* mCamNode = nullptr;

	static constexpr float mCameraSpeed = 50.f;
	static constexpr float mZoomSpeed = 20.f;
	static constexpr float mClampMinY = 20.f;
	static constexpr float mClampMaxY = 175.f;
	static constexpr float mClampMaxX = 300.f;
	static constexpr float mClampMinZ = -55.f;
	static constexpr float mClampMaxZ = 300.f;
	float mMoveX = 0.f;
	float mMoveZ = 0.f;
	int mMouseWheelY = 0;
};

