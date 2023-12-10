#include "Globals.h"
#include "Application.h"

#include "ModuleCamera.h"
#include "Model.h"
#include "ModuleWindow.h"

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
    // Retrieve model view and projection
    frustum->type = FrustumType::PerspectiveFrustum;
    frustum->pos = position;
    frustum->front = front;
    frustum->up = up;
    frustum->nearPlaneDistance = 0.01f;
    frustum->farPlaneDistance = 1000.0f;
    frustum->verticalFov = math::pi / 4.0f;
    frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) * (App->GetWindow()->GetScreenWidth() / App->GetWindow()->GetScreenHeight()));

	return true;
}

update_status ModuleCamera::PreUpdate()
{
    frustum->type = FrustumType::PerspectiveFrustum;
    frustum->pos = position;
    frustum->front = front;
    frustum->up = up;
    frustum->nearPlaneDistance = 0.01f;
    frustum->farPlaneDistance = 1000.0f;
    //frustum->verticalFov = math::pi / 4.0f;
    frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov * 0.5f) * (App->GetWindow()->GetScreenWidth() / App->GetWindow()->GetScreenHeight()));

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
    static Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 currentFrameTime = SDL_GetTicks();
    float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; 
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

    case SDLK_LCTRL:
        if (keyEvent.type == SDL_KEYDOWN)
            speedDecrease = true;
        else if (keyEvent.type == SDL_KEYUP)
            speedDecrease = false;
        break;

    case SDLK_LSHIFT:
        if (keyEvent.type == SDL_KEYDOWN)
            speedIncrease = true;
        else if (keyEvent.type == SDL_KEYUP)
            speedIncrease = false;
        break;

    case SDLK_f:
        if (keyEvent.type == SDL_KEYDOWN)
            lookAtCenter = true;
        else if (keyEvent.type == SDL_KEYUP)
            lookAtCenter = false;
        break;

    default:
        // Handle other keys if needed
        break;
    }
}

void ModuleCamera::UpdatePosition(float deltaTime) {
    //speed = 5.0f;

    // Adjust speed based on delta time
    float adjustedSpeed = speed * deltaTime;
    if (speedIncrease) adjustedSpeed *= 2;
    if (speedDecrease) adjustedSpeed /= 2;

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
        
        if (deltaX != 0) {
            float angle = -deltaX * sensitivity;
            RotateYaw(angle);
        }
        if (deltaY != 0) {
            float angle = -deltaY * sensitivity;
            RotatePitch(angle);
        }

        lastMouseX = motionEvent.x;
        lastMouseY = motionEvent.y;

        if (lookAtCenter)
        {
            // Orbit around the center point
            float3 orbitAxis = float3(0.0f, 1.0f, 0.0f); // You can change the orbit axis as needed
            Quat rotationYaw = Quat::RotateAxisAngle(orbitAxis, deltaX * sensitivity);
            Quat rotationPitch = Quat::RotateAxisAngle(frustum->WorldRight(), deltaY * sensitivity);

            // Calculate the vector from the center to the camera
            float3 toCamera = position - float3::zero;

            // Rotate the position around the orbit axis
            float3 rotatedPosition = rotationYaw * rotationPitch * toCamera;

            // Set the new position relative to the center
            position = float3::zero + rotatedPosition;

            // Ensure the camera looks at the center
            LookAt(float3::zero);
        }
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

void ModuleCamera::HandleMouseWheel(const SDL_MouseWheelEvent& wheelEvent)
{
    if (wheelEvent.y > 0) // Scrolling up
    {
        // Zoom in
        frustum->verticalFov -= zoomSpeed;
        frustum->verticalFov = math::Clamp(frustum->verticalFov, 0.1f, math::pi - 0.1f);
    }
    else if (wheelEvent.y < 0) // Scrolling down
    {
        // Zoom out
        frustum->verticalFov += zoomSpeed;
        frustum->verticalFov = math::Clamp(frustum->verticalFov, 0.1f, math::pi - 0.1f);
    }
}

void ModuleCamera::LookAt(const float3& target_pos)
{
    // Calculate new front vector
    front = (target_pos - position).Normalized();

    // Assume up vector is initially (0, 1, 0)
    float3 worldUp(0.0f, 1.0f, 0.0f);

    // Calculate new right vector
    float3 right = worldUp.Cross(front).Normalized();

    // Calculate new up vector
    up = front.Cross(right).Normalized();
}