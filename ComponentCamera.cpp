#include "ComponentCamera.h"
#include "GameObject.h"
#include "Math/Quat.h"
#include "imgui.h"

ComponentCamera::ComponentCamera(GameObject* owner) : Component(owner, ComponentType::CAMERA)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 7.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 60.0f, 1.3f);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::RenderToEditor()
{
	ImGui::Separator();
	ImGui::Checkbox("Activate Frustum Culling", &isCullingActive);
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

void ComponentCamera::SetFrustum(Frustum newFrustum)
{
	frustum = newFrustum;
}

