#pragma once

#include <string>

class GameObject;

class Component
{
public:
	enum ComponentType {
		MATERIAL,
		MESH,
		TRANSFORM,
		UNDEFINED
	};

	Component();
	Component(GameObject* owner, ComponentType type);
	~Component();

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	void SetParent(GameObject* parent);

private:
	ComponentType type = ComponentType::UNDEFINED;
	std::string uid;
	bool enabled = true;
	GameObject* owner;
};

