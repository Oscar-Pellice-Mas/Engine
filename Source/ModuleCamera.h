#ifndef _MODULE_CAMERA_H_
#define _MODULE_CAMERA_H_

#include "Module.h"

#include "MathGeoLib/include/Math/float3.h"
#include "SDL/include/SDL.h"


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void HandleKeyboardInput(const SDL_KeyboardEvent& keyEvent);
	void HandleMouseMotion(const SDL_MouseMotionEvent& motionEvent);
	void HandleMouseButton(const SDL_MouseButtonEvent& buttonEvent);

	Frustum* frustum = nullptr;

private:
	void UpdatePosition(float deltaTime);
	void RotatePitch(float angle);
	void RotateYaw(float angle);

	bool moveUp = false;
	bool moveDown = false;
	bool moveForward = false;
	bool moveBackward = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool rotatePitchUp = false;
	bool rotatePitchDown = false;
	bool rotateYawLeft = false;
	bool rotateYawRight = false;

	float3 position = float3(0.0f, 1.0f, 5.0f);
	float3 front = -float3::unitZ;
	float3 up = float3::unitY;

	int lastMouseX = 0;
	int lastMouseY = 0;
	bool isMouseDragging = false;
};
#endif /*_MODULE_CAMERA_H_*/