#include "ComponentCamera.h"
#include "GameObject.h"
#include "Math/Quat.h"

ComponentCamera::ComponentCamera(GameObject* owner) : Component(owner, ComponentType::CAMERA)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 10.0f);
	//frustum.SetPerspective(1.0f, 1.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, 1.3f);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::RenderToEditor()
{
}

Frustum ComponentCamera::GetFrustum() const
{
	return frustum;
}

bool ComponentCamera::GetCullingStatus() const
{
	return isCullingActive;
}

void ComponentCamera::SetNewPosition(float3 newPosition)
{
	frustum.SetPos(newPosition);
}

