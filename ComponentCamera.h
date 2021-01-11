#pragma once

#include "Component.h"
#include "Globals.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"

class ComponentCamera :  public Component
{
public:
	ComponentCamera(GameObject* owner);
	~ComponentCamera();

	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFOV() const;
	float GetAspectRatio() const;
	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void SetFOV(float fov);
	void SetAspectRatio(float aspectRatio);
	float4x4 GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;

	Frustum frustum;
};

