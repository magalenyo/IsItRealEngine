#pragma once
#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"

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
	void SetNearPlaneDistance(float dist);
	void SetFarPlaneDistance(float dist);
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

private:
	Frustum frustum;
	float movementSpeed = 5.0f;
	float rotationSpeed = 60.0f;
	float zoomSpeed = 5.0f;
	float positionFromFocusVertice = 4.0f;
	float distanceFocus = 10.0f;

	void ResetSpeed();
	void IncreaseSpeed();
	void Rotate(float3x3 rotationMatrix);
};
