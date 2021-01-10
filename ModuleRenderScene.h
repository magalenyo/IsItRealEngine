#pragma once
#include "Module.h"

class GameObject;

class ModuleRenderScene : public Module
{
public:
	update_status Update() override;

private:
	void RenderGameObjectsRecursively(const GameObject* node) const;
}; 

