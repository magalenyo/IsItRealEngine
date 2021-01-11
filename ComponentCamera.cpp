#include "ComponentCamera.h"
#include "GameObject.h"

ComponentCamera::ComponentCamera(GameObject* owner)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0, 0, 0));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 1000.0f);
	frustum.SetPerspective(1.0f, 1.0f);

}

ComponentCamera::~ComponentCamera()
{
}

float ComponentCamera::GetNearPlane() const
{
	return frustum.NearPlaneDistance();
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.FarPlaneDistance();
}

float ComponentCamera::GetFOV() const
{
	return frustum.VerticalFov();
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetNearPlane(float distance)
{
	frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
}

void ComponentCamera::SetFarPlane(float distance)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);
}

void ComponentCamera::SetFOV(float fov)
{
	frustum.SetVerticalFovAndAspectRatio(fov, frustum.AspectRatio());
}

void ComponentCamera::SetAspectRatio(float aspectRatio)
{
	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), aspectRatio);
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

float4x4 ComponentCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}
