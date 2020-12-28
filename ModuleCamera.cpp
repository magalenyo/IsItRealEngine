#include "ModuleCamera.h"
#include "Geometry/Frustum.h"
#include "GL/glew.h"
#include "SDL.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "Math/float3x3.h"
#include "MemoryLeakDetector.h"

bool ModuleCamera::Init()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1f, 200.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	frustum.SetPos(float3(0, 3, -8));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	return true;
}

void ModuleCamera::Rotate(float3x3 rotationMatrix) {
	frustum.SetFront(rotationMatrix * frustum.Front().Normalized());
	frustum.SetUp(rotationMatrix * frustum.Up().Normalized());
}

update_status ModuleCamera::Update()
{
	const float deltaTime = App->time->DeltaTime();
	const float2& mouseMotion = App->input->GetMouseMotion();
	const float mouseWheelMotion = App->input->GetMouseWheelMotion();

	/* START SPEED CONTROL */
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT) {
		IncreaseSpeed();
	}
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
		ResetSpeed();
	}
	/* END SPEED CONTROL */

	if (mouseWheelMotion < -FLT_EPSILON || mouseWheelMotion > FLT_EPSILON) {
		frustum.SetPos(frustum.Pos() + (frustum.Front().Normalized() * mouseWheelMotion * 10 * zoomSpeed * App->time->DeltaTime()));
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT) {
		Rotate(float3x3::RotateAxisAngle(frustum.WorldRight().Normalized(), -mouseMotion.y * rotationSpeed * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateY(-mouseMotion.x * rotationSpeed * DEGTORAD * deltaTime));

		if (App->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
		{
			frustum.SetPos(frustum.Pos() + (frustum.Up().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
		{
			frustum.SetPos(frustum.Pos() + (frustum.Up().Normalized() * movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			frustum.SetPos(frustum.Pos() + (frustum.Front().Normalized() * movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			frustum.SetPos(frustum.Pos() + (frustum.Front().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			frustum.SetPos(frustum.Pos() + (frustum.WorldRight().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			frustum.SetPos(frustum.Pos() + (frustum.WorldRight().Normalized() * movementSpeed * deltaTime));
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_REPEAT) {
		frustum.SetPos(float3(0, 3, -8));
		frustum.SetFront(float3::unitZ);
		frustum.SetUp(float3::unitY);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
		float3 oldFocusPoint = frustum.Pos() + (frustum.Front() * distanceFocus);
		Rotate(float3x3::RotateAxisAngle(frustum.WorldRight().Normalized(), -mouseMotion.y * rotationSpeed * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateY(-mouseMotion.x * rotationSpeed * DEGTORAD * deltaTime));
		float3 newFocusPoint = frustum.Pos() + (frustum.Front() * distanceFocus);
		frustum.SetPos((oldFocusPoint - newFocusPoint) + frustum.Pos());
	}

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}


void ModuleCamera::ResetSpeed()
{
	movementSpeed = 5.0f;
	rotationSpeed = 25.0f;
}

void ModuleCamera::IncreaseSpeed()
{
	movementSpeed *= 2.0f;
	rotationSpeed *= 2.0f;
}

void ModuleCamera::OnWindowResized(int width, int height)
{
	frustum.SetVerticalFovAndAspectRatio(frustum.VerticalFov(), ((float) width) / (float) height);
}

void ModuleCamera::SetHorizontalFov(int fov)
{
	frustum.SetHorizontalFovAndAspectRatio(fov, frustum.AspectRatio());
}

void ModuleCamera::SetVerticalFov(int fov)
{
	frustum.SetVerticalFovAndAspectRatio(fov, frustum.AspectRatio());
}

void ModuleCamera::SetNearPlaneDistance(float dist)
{
	frustum.SetViewPlaneDistances(dist, frustum.FarPlaneDistance());
}

void ModuleCamera::SetFarPlaneDistance(float dist)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), dist);
}

void ModuleCamera::FocusCamera(const float3 &position)
{
	frustum.SetPos(float3(position.x, position.y, position.z + positionFromFocusVertice));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

float4x4 ModuleCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

vec ModuleCamera::GetFront() const
{
	return frustum.Front();
}

vec ModuleCamera::GetUp() const
{
	return frustum.Up();
}

vec ModuleCamera::GetPosition() const
{
	return frustum.Pos();
}

float ModuleCamera::GetNearPlane() const
{
	return frustum.NearPlaneDistance();
}

float ModuleCamera::GetFarPlane() const
{
	return frustum.FarPlaneDistance();
}

float ModuleCamera::GetFOV() const
{
	return frustum.VerticalFov();
}

float ModuleCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

float ModuleCamera::GetMovementSpeed() const
{
	return movementSpeed;
}

float ModuleCamera::GetRotationSpeed() const
{
	return rotationSpeed;
}

float ModuleCamera::GetZoomSpeed() const
{
	return zoomSpeed;
}