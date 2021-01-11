#include "ModuleCamera.h"
#include "Geometry/Frustum.h"
#include "GL/glew.h"
#include "SDL.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleRender.h"
#include "Math/float3x3.h"
#include "MemoryLeakDetector.h"

ModuleCamera::ModuleCamera()
{
	camera = new ComponentCamera(nullptr);

	camera->frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	camera->frustum.SetViewPlaneDistances(0.1f, 200.0f);
	camera->frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);

	camera->frustum.SetPos(float3(0, 3, -8));
	camera->frustum.SetFront(float3::unitZ);
	camera->frustum.SetUp(float3::unitY);
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	App->renderer->mainCamera = camera;
	return true;
}

void ModuleCamera::Rotate(float3x3 rotationMatrix) {
	camera->frustum.SetFront(rotationMatrix * camera->frustum.Front().Normalized());
	camera->frustum.SetUp(rotationMatrix * camera->frustum.Up().Normalized());
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
		camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.Front().Normalized() * mouseWheelMotion * 10 * zoomSpeed * App->time->DeltaTime()));
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT) {
		Rotate(float3x3::RotateAxisAngle(camera->frustum.WorldRight().Normalized(), -mouseMotion.y * rotationSpeed * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateY(-mouseMotion.x * rotationSpeed * DEGTORAD * deltaTime));

		if (App->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
		{
			camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.Up().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
		{
			camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.Up().Normalized() * movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.Front().Normalized() * movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.Front().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.WorldRight().Normalized() * -movementSpeed * deltaTime));
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			camera->frustum.SetPos(camera->frustum.Pos() + (camera->frustum.WorldRight().Normalized() * movementSpeed * deltaTime));
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_REPEAT) {
		camera->frustum.SetPos(float3(0, 3, -8));
		camera->frustum.SetFront(float3::unitZ);
		camera->frustum.SetUp(float3::unitY);
	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
		float3 oldFocusPoint = camera->frustum.Pos() + (camera->frustum.Front() * distanceFocus);
		Rotate(float3x3::RotateAxisAngle(camera->frustum.WorldRight().Normalized(), -mouseMotion.y * rotationSpeed * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateY(-mouseMotion.x * rotationSpeed * DEGTORAD * deltaTime));
		float3 newFocusPoint = camera->frustum.Pos() + (camera->frustum.Front() * distanceFocus);
		camera->frustum.SetPos((oldFocusPoint - newFocusPoint) + camera->frustum.Pos());
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
	camera->frustum.SetVerticalFovAndAspectRatio(camera->frustum.VerticalFov(), ((float) width) / (float) height);
}

void ModuleCamera::FocusCamera(const float3 &position)
{
	camera->frustum.SetPos(float3(position.x, position.y, position.z + positionFromFocusVertice));
	camera->frustum.SetFront(float3::unitZ);
	camera->frustum.SetUp(float3::unitY);
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

ComponentCamera* ModuleCamera::GetCamera() const
{
	return camera;
}

