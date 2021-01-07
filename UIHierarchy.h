#pragma once
#include "UIComponent.h"

class GameObject;

class UIHierarchy : public UIComponent
{
public:
	void Draw() override;

private:
	void RenderRecursively(GameObject* gameObject);

	GameObject* clickedNode = nullptr;
};

