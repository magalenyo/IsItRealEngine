#pragma once
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Math/Quat.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 position, float3 scale, Quat rotation, GameObject* owner);
	~ComponentTransform() {};

	void SetPosition(float3 pos) { position = pos; }
	void SetScale(float3 sca) { scale = sca; }
	void SetRotation(Quat rot) { rotation = rot; }

	void Reset() { ResetPosition(); ResetScale(); ResetRotation(); }
	void ResetPosition() { position = float3(0, 0, 0); }
	void ResetScale()    { scale	= float3(1, 1, 1); }
	void ResetRotation() { rotation = Quat(0, 0, 0, 0); }

	float3 GetPosition() const { return position; }
	float3 GetScale() const { return scale; }
	Quat GetRotation() const { return rotation; };

	float3 GetRotationMatrix() const { return rotation.ToEulerXYZ(); };
	float4x4 GetGlobalModelMatrix() const { return globalMatrix; }

private:

	float3 position = float3(0, 0, 0);		// Local Matrix position
	float3 scale	= float3(1, 1, 1);		// Local Matrix scale
	Quat rotation	= Quat(0, 0, 0, 0);		// Local Matrix rotation

	float4x4 localMatrix = float4x4::identity;				// Local Model Matrix
	float4x4 globalMatrix = float4x4::identity;				// Global Model Matrix
};

