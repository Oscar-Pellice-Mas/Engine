#ifndef _MODULE_CAMERA_H_
#define _MODULE_CAMERA_H_

#include "Module.h"

#include "MathGeoLib/include/Math/float3.h"
#include "SDL/include/SDL.h"

class Model;

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
	void HandleMouseWheel(const SDL_MouseWheelEvent& wheelEvent);
	
	void SetPosition(float3 pos) { position = pos; }
	void LookAt(const float3& target_pos);

	void SetSpeed(float spd) { speed = spd; }
	void SetSensitivity(float sens) { sensitivity = sens; }
	void SetZoomSpeed(float speed) { zoomSpeed = speed; }

	float GetSpeed() { return speed; }
	float GetSensitivity() { return sensitivity; }
	float GetZoomSpeed() { return zoomSpeed; }

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
	bool speedIncrease = false;
	bool speedDecrease = false;
	bool lookAtCenter = false;

	float orbitalSpeed = 0.1f;

	float3 position = float3(0.0f, 1.0f, 5.0f);
	float3 front = -float3::unitZ;
	float3 up = float3::unitY;

	int lastMouseX = 0;
	int lastMouseY = 0;
	bool isMouseDragging = false;

	float speed = 5.0f; 
	float sensitivity = 0.01f;
	float zoomSpeed = 0.1f;
};
#endif /*_MODULE_CAMERA_H_*/