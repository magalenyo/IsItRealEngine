#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"
#include "assimp/scene.h"

typedef unsigned __int8 Uint8;


class ModuleCamera : public Module
{
public:
	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void OnWindowResized(int width, int height);
	void SetHorizontalFov(int fov);
	void SetVerticalFov(int fov);
	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void FocusCamera(const float3 &position);
	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;
	vec GetFront() const;
	vec GetUp() const;
	vec GetPosition() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFOV() const;
	float GetAspectRatio() const;
	float GetMovementSpeed() const;
	float GetRotationSpeed() const;
	float GetZoomSpeed() const;

	Frustum GetCamera() const;

private:
	Frustum camera = Frustum();
	float movementSpeed = 5.0f;
	float rotationSpeed = 60.0f;
	float zoomSpeed = 5.0f;
	float positionFromFocusVertice = 4.0f;
	float distanceFocus = 10.0f;

	void ResetSpeed();
	void IncreaseSpeed();
	void Rotate(float3x3 rotationMatrix);
	GameObject* Pick() const;
};
