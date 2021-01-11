#pragma once
#include "Module.h"
#include "Globals.h"
#include "ComponentCamera.h"
#include "Math/float4x4.h"

typedef unsigned __int8 Uint8;


class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();
	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void OnWindowResized(int width, int height);
	void FocusCamera(const float3 &position);
	float GetMovementSpeed() const;
	float GetRotationSpeed() const;
	float GetZoomSpeed() const;
	ComponentCamera* GetCamera() const;

private:
	ComponentCamera* camera = nullptr;
	float movementSpeed = 5.0f;
	float rotationSpeed = 60.0f;
	float zoomSpeed = 5.0f;
	float positionFromFocusVertice = 4.0f;
	float distanceFocus = 10.0f;

	void ResetSpeed();
	void IncreaseSpeed();
	void Rotate(float3x3 rotationMatrix);
};
