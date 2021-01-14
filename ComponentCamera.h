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

	void RenderToEditor() override;

	Frustum GetFrustum() const;
	bool GetCullingStatus() const;

	void SetNewPosition(float3 newPosition);

private:
	Frustum frustum = Frustum();
	bool isCullingActive = false;
};

