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
	void RenderGameObjectsRecursively(const GameObject* node) const; //Iterates from the object passed by parameter and renders himself and all its children. If frustum culling is activated, only renders the object that collide with the scene's camera's frustum
	std::vector<GameObject*> TestCulling(const std::vector<GameObject*> candidatesToCull, Frustum frustum) const; //Returns all the objects from the scene that collide with the frustum passed by parameter in order to render them.
}; 

