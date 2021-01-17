#include "ModuleCamera.h"
#include "Geometry/Frustum.h"
#include "GL/glew.h"
#include "SDL.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleRender.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Math/float3x3.h"
#include "Geometry/LineSegment.h"
#include "MemoryLeakDetector.h"

bool ModuleCamera::Init()
{
	camera.SetKind(FrustumSpaceGL, FrustumRightHanded);
	camera.SetViewPlaneDistances(0.1f, 200.0f);
	camera.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	camera.SetPos(float3(0, 3, -8));
	camera.SetFront(float3::unitZ);
	camera.SetUp(float3::unitY);

	return true;
}

void ModuleCamera::Rotate(float3x3 rotationMatrix) {
	camera.SetFront(rotationMatrix * camera.Front().Normalized());
	camera.SetUp(rotationMatrix * camera.Up().Normalized());
}

GameObject* ModuleCamera::Pick() const
{
	float width = App->renderer->viewportWidth;
	float height = App->renderer->viewportHeight;

	const float2& mouse = App->input->GetMousePosition();
	const float2& mouse2 = App->editor->GetSceneviewPosition();

	float normalizedX = -(1.0f - (float(mouse2.x) * 2.0f) / width);
	float normalizedY = 1.0f - (float(mouse2.y) * 2.0f) / height;

	LineSegment picking = camera.UnProjectLineSegment(normalizedX, normalizedY);

	float distance;
	GameObject* picked = App->scene->SendRay(picking, distance);

	return picked;
}

update_status ModuleCamera::Update()
{
	const float deltaTime = App->time->DeltaTime();
	const float2& mouseMotion = App->input->GetMouseMotion();
	const float mouseWheelMotion = App->input->GetMouseWheelMotion();

	/* START SPEED CONTROL */
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT)
	{
		IncreaseSpeed();
	}
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) 
	{
		ResetSpeed();
	}
	/* END SPEED CONTROL */

	if (mouseWheelMotion < -FLT_EPSILON || mouseWheelMotion > FLT_EPSILON) 
	{
		camera.SetPos(camera.Pos() + (camera.Front().Normalized() * mouseWheelMotion * 10 * zoomSpeed * App->time->DeltaTime()));
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT) 
	{
		Rotate(float3x3::RotateAxisAngle(camera.WorldRight().Normalized(), -mouseMotion.y * rotationSpeed * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateY(-mouseMotion.x * rotationSpeed * DEGTORAD * deltaTime));

		if (App->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
		{
			camera.SetPos(camera.Pos() + (camera.Up().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
		{
			camera.SetPos(camera.Pos() + (camera.Up().Normalized() * movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			camera.SetPos(camera.Pos() + (camera.Front().Normalized() * movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			camera.SetPos(camera.Pos() + (camera.Front().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			camera.SetPos(camera.Pos() + (camera.WorldRight().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			camera.SetPos(camera.Pos() + (camera.WorldRight().Normalized() * movementSpeed * deltaTime));
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_REPEAT) 
	{
		camera.SetPos(float3(0, 3, -8));
		camera.SetFront(float3::unitZ);
		camera.SetUp(float3::unitY);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) 
	{
		float3 oldFocusPoint = camera.Pos() + (camera.Front() * distanceFocus);
		Rotate(float3x3::RotateAxisAngle(camera.WorldRight().Normalized(), -mouseMotion.y * rotationSpeed * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateY(-mouseMotion.x * rotationSpeed * DEGTORAD * deltaTime));
		float3 newFocusPoint = camera.Pos() + (camera.Front() * distanceFocus);
		camera.SetPos((oldFocusPoint - newFocusPoint) + camera.Pos());
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
	{
		GameObject* picked = Pick();
		if (picked != nullptr)
		{
			App->editor->SetSelectedGameObject(picked);
		}
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
	camera.SetVerticalFovAndAspectRatio(camera.VerticalFov(), ((float) width) / (float) height);
}

void ModuleCamera::SetHorizontalFov(int fov)
{
	camera.SetHorizontalFovAndAspectRatio(fov, camera.AspectRatio());
}

void ModuleCamera::SetVerticalFov(int fov)
{
	camera.SetVerticalFovAndAspectRatio(fov, camera.AspectRatio());
}

void ModuleCamera::SetNearPlane(float distance)
{
	camera.SetViewPlaneDistances(distance, camera.FarPlaneDistance());
}

void ModuleCamera::SetFarPlane(float distance)
{
	camera.SetViewPlaneDistances(camera.NearPlaneDistance(), distance);
}

void ModuleCamera::FocusCamera(const float3 &position)
{
	camera.SetPos(float3(position.x, position.y, position.z + positionFromFocusVertice));
	camera.SetFront(float3::unitZ);
	camera.SetUp(float3::unitY);
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	return camera.ViewMatrix();
}

float4x4 ModuleCamera::GetProjectionMatrix() const
{
	return camera.ProjectionMatrix();
}

vec ModuleCamera::GetFront() const
{
	return camera.Front();
}

vec ModuleCamera::GetUp() const
{
	return camera.Up();
}

vec ModuleCamera::GetPosition() const
{
	return camera.Pos();
}

float ModuleCamera::GetNearPlane() const
{
	return camera.NearPlaneDistance();
}

float ModuleCamera::GetFarPlane() const
{
	return camera.FarPlaneDistance();
}

float ModuleCamera::GetFOV() const
{
	return camera.VerticalFov();
}

float ModuleCamera::GetAspectRatio() const
{
	return camera.AspectRatio();
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

Frustum ModuleCamera::GetCamera() const
{
	return camera;
}

