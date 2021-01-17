#pragma once
#include "UIComponent.h"

class GameObject;

class UIHierarchy : public UIComponent
{
public:
	void Draw() override;
	GameObject* GetSelectedGameObject() { return selectedGameObject; };
	void SetSelectedGameObject(GameObject* newSelected);

private:
	void RenderRecursively(GameObject* gameObject);
	void RenderActionsForGameObject(GameObject* gameObject);

	GameObject* selectedGameObject = nullptr;
	unsigned int emptyGameobjectCounter = 0;
};

