#pragma once
#include "Module.h"
#include <vector>
#include "Geometry/Frustum.h"

class GameObject;

class ModuleRenderScene : public Module
{
public:
	update_status Update() override;

private:
	void RenderGameObjectsRecursively(const GameObject* node) const;
	std::vector<GameObject*> TestCulling(const std::vector<GameObject*> candidatesToCull, Frustum frustum) const;
}; 

