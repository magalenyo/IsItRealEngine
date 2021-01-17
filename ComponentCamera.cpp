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

	frustum.SetViewPlaneDistances(0.1f, 10.0f);
	frustum.SetHorizontalFovAndAspectRatio(DEGTORAD * 60.0f, 1.3f);
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::RenderToEditor()
{
	ImGui::Separator();
	ImGui::Checkbox("Activate Frustum Culling", &isCullingActive);

	
	float newFarPlaneDistance = frustum.FarPlaneDistance();
	float newNearPlaneDistance = frustum.NearPlaneDistance();
	bool planesChanged = ImGui::DragFloat("Far plane distance", &newFarPlaneDistance, 1.0f, newNearPlaneDistance + 5, FLT_MAX, "%.3f");
	   planesChanged |= ImGui::DragFloat("Near plane distance", &newNearPlaneDistance, 1.0f, 0.1f, newFarPlaneDistance - 5, "%.3f");

	if (planesChanged) {
		frustum.SetViewPlaneDistances(newNearPlaneDistance, newFarPlaneDistance);
	}
	
	float horizontalFov = frustum.HorizontalFov() * RADTODEG;
	if (ImGui::DragFloat("Horizontal fov", &horizontalFov, .5f, 20, FLT_MAX, "%.3f")) {
		frustum.SetHorizontalFovAndAspectRatio(horizontalFov * DEGTORAD, frustum.AspectRatio());
	}
	
	//ImGui::PushID("Ty"); ImGui::DragFloat("y", &position.y, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
	//ImGui::PushID("Tz"); ImGui::DragFloat("z", &position.z, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
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

