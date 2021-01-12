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
	virtual ~Component();

	virtual void Enable() { enabled = true; }
	virtual void Disable() { enabled = false; }
	virtual void Update() {}

	void SetParent(GameObject* parent);
	
	std::string GetUID() const { return uid; };

private:
	ComponentType type = ComponentType::UNDEFINED;
	std::string uid;
	bool enabled = true;
	GameObject* owner = nullptr;
};

