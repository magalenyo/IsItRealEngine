#pragma once
#include "Math/float3.h"

class Transformation
{
public:
	Transformation() {};
	~Transformation() {};

	void SetPosition(float3 pos) { position = pos; }
	void SetScale(float3 sca) { scale = sca; }
	void SetRotation(float3 rot) { rotation = rot; }

	void Reset() { ResetPosition(); ResetScale(); ResetRotation(); }
	void ResetPosition() { position = float3(0, 0, 0); }
	void ResetScale()    { scale	= float3(1, 1, 1); }
	void ResetRotation() { rotation = float3(0, 0, 0); }

	float3 GetPosition() const { return position; }
	float3 GetScale() const { return scale; }
	float3 GetRotation() const { return rotation; };

private:

	float3 position = float3(0, 0, 0);
	float3 scale	= float3(1, 1, 1);
	float3 rotation = float3(0, 0, 0);
};

