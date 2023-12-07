#include "Globals.h"
#include "Application.h"

#include "ModuleCamera.h"

#include "SDL.h"
#include <GL/glew.h>
#include "MathGeoLib.h"

ModuleCamera::ModuleCamera()
{
    frustum = new Frustum();
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
    /*frustum->pos = float3(0.0f, 1.0f, 5.0f);;
    frustum->front = -float3::unitZ;
    frustum->up = float3::unitY;
    frustum->type = FrustumType::PerspectiveFrustum;
    frustum->nearPlaneDistance = 0.1f;
    frustum->farPlaneDistance = 100.0f;
    frustum->verticalFov = math::pi / 4.0f;
    frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f)) * 1.3f;*/

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	// Retrieve model view and projection
	frustum->type = FrustumType::PerspectiveFrustum;
    frustum->pos = position;
    frustum->front = front;
	frustum->up = up;
	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 100.0f;
	frustum->verticalFov = math::pi / 4.0f;
	frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f)) * 1.3f;

    float4x4 proj = frustum->ProjectionMatrix();

    float4x4 model = float4x4::identity;
	/*model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),
		float4x4::RotateZ(pi / 4.0f),
		float3(2.0f, 1.0f, 1.0f));*/

    float4x4 view = frustum->ViewMatrix();
	//view = float4x4::LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY, frustum.up);

	//glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	//glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	//glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
    static Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = SDL_GetTicks();
    float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Convert to seconds
    lastFrameTime = currentFrameTime;

    UpdatePosition(deltaTime);

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleCamera::CleanUp()
{

	return true;
}


void ModuleCamera::HandleKeyboardInput(const SDL_KeyboardEvent& keyEvent)
{
    switch (keyEvent.keysym.sym)
    {
    case SDLK_q:
        if (keyEvent.type == SDL_KEYDOWN)
            moveUp = true;
        else if (keyEvent.type == SDL_KEYUP)
            moveUp = false;
        break;

    case SDLK_e:
        if (keyEvent.type == SDL_KEYDOWN)
            moveDown = true;
        else if (keyEvent.type == SDL_KEYUP)
            moveDown = false;
        break;

    case SDLK_w:
        if (keyEvent.type == SDL_KEYDOWN)
            moveForward = true;
        else if (keyEvent.type == SDL_KEYUP)
            moveForward = false;
        break;

    case SDLK_s:
        if (keyEvent.type == SDL_KEYDOWN)
            moveBackward = true;
        else if (keyEvent.type == SDL_KEYUP)
            moveBackward = false;
        break;

    case SDLK_a:
        if (keyEvent.type == SDL_KEYDOWN)
            moveLeft = true;
        else if (keyEvent.type == SDL_KEYUP)
            moveLeft = false;
        break;

    case SDLK_d:
        if (keyEvent.type == SDL_KEYDOWN)
            moveRight = true;
        else if (keyEvent.type == SDL_KEYUP)
            moveRight = false;
        break;

    case SDLK_UP:
        if (keyEvent.type == SDL_KEYDOWN)
            rotatePitchUp = true;
        else if (keyEvent.type == SDL_KEYUP)
            rotatePitchUp = false;
        break;

    case SDLK_DOWN:
        if (keyEvent.type == SDL_KEYDOWN)
            rotatePitchDown = true;
        else if (keyEvent.type == SDL_KEYUP)
            rotatePitchDown = false;
        break;

    case SDLK_LEFT:
        if (keyEvent.type == SDL_KEYDOWN)
            rotateYawLeft = true;
        else if (keyEvent.type == SDL_KEYUP)
            rotateYawLeft = false;
        break;

    case SDLK_RIGHT:
        if (keyEvent.type == SDL_KEYDOWN)
            rotateYawRight = true;
        else if (keyEvent.type == SDL_KEYUP)
            rotateYawRight = false;
        break;

    default:
        // Handle other keys if needed
        break;
    }
}

void ModuleCamera::UpdatePosition(float deltaTime) {
    float speed = 5.0f; // Adjust the speed as needed

    // Adjust speed based on delta time
    float adjustedSpeed = speed * deltaTime;

    // Update position based on the state of movement directions
    position += (moveUp ? float3(0.0f, adjustedSpeed, 0.0f) : float3(0.0f, 0.0f, 0.0f))
        - (moveDown ? float3(0.0f, adjustedSpeed, 0.0f) : float3(0.0f, 0.0f, 0.0f))
        + (moveForward ? adjustedSpeed * frustum->front : float3(0.0f, 0.0f, 0.0f))
        - (moveBackward ? adjustedSpeed * frustum->front : float3(0.0f, 0.0f, 0.0f))
        - (moveLeft ? adjustedSpeed * frustum->WorldRight() : float3(0.0f, 0.0f, 0.0f))
        + (moveRight ? adjustedSpeed * frustum->WorldRight() : float3(0.0f, 0.0f, 0.0f));
    
    // Rotate based on the state of rotation directions
    if (rotatePitchUp)
        RotatePitch(adjustedSpeed);
    if (rotatePitchDown)
        RotatePitch(-adjustedSpeed);
    if (rotateYawLeft)
        RotateYaw(-adjustedSpeed);
    if (rotateYawRight)
        RotateYaw(adjustedSpeed);
}

void ModuleCamera::RotatePitch(float angle)
{
    // Generate a rotation matrix or quaternion for pitch rotation
    float4x4 rotationMatrix = float4x4::RotateAxisAngle(frustum->WorldRight(), angle);

    // Apply the rotation to the frustum's up and front vectors
    up = rotationMatrix.MulDir(frustum->up).Normalized();
    front = rotationMatrix.MulDir(frustum->front).Normalized();
}

void ModuleCamera::RotateYaw(float angle)
{
    // Generate a rotation matrix or quaternion for yaw rotation
    float4x4 rotationMatrix = float4x4::RotateAxisAngle(float3(0.0f, 1.0f, 0.0f), angle);

    // Apply the rotation to the frustum's up and front vectors
    up = rotationMatrix.MulDir(frustum->up).Normalized();
    front = rotationMatrix.MulDir(frustum->front).Normalized();
}

void ModuleCamera::HandleMouseMotion(const SDL_MouseMotionEvent& motionEvent)
{
    if (isMouseDragging)
    {
        int deltaX = motionEvent.x - lastMouseX;
        int deltaY = motionEvent.y - lastMouseY;

        const float sensitivity = 0.01f;
        if (deltaX != 0) {
            RotateYaw(-deltaX * sensitivity);
        }
        if (deltaY != 0) {
            RotatePitch(-deltaY * sensitivity);
        }

        lastMouseX = motionEvent.x;
        lastMouseY = motionEvent.y;
    }
}

void ModuleCamera::HandleMouseButton(const SDL_MouseButtonEvent& buttonEvent)
{
    if (buttonEvent.button == SDL_BUTTON_RIGHT)
    {
        if (buttonEvent.type == SDL_MOUSEBUTTONDOWN)
        {
            isMouseDragging = true;
            SDL_GetMouseState(&lastMouseX, &lastMouseY);
        }
        else if (buttonEvent.type == SDL_MOUSEBUTTONUP)
        {
            isMouseDragging = false;
        }
    }
}