#pragma once

#include <string>

class GameObject;

class Component
{
public:
	enum class ComponentType {
		MATERIAL,
		MESH,
		TRANSFORM,
		CAMERA,
		UNDEFINED
	};

	Component();
	Component(GameObject* owner, ComponentType type);
	~Component();

	virtual void Enable() { enabled = true; }
	virtual void Disable() { enabled = false; }
	virtual void Update() {}

	void SetParent(GameObject* parent);

private:
	ComponentType type = ComponentType::UNDEFINED;
	std::string uid;
	bool enabled = true;
	GameObject* owner = nullptr;
};

