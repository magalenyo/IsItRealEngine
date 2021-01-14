#pragma once
#include "Module.h"
#include "Application.h"
#include "GameObject.h"
#include "Quadtree.h"


class GameObject;

class ModuleRenderScene : public Module
{
public:
	ModuleRenderScene();
	~ModuleRenderScene();
	bool Init() override;
	update_status Update() override;

private:
	void RenderGameObjectsRecursively(const GameObject* node);
	std::vector<GameObject*> TestCulling(const std::vector<GameObject*> candidatesToCull, Frustum frustum);
}; 

