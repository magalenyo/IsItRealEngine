#pragma once

#include <string>

class GameObject;

class Component
{
public:
	enum ComponentType {
		Material,
		Mesh,
		Transform
	};

	Component();
	Component(GameObject* owner, ComponentType type);
	~Component();

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

private:
	ComponentType type;
	std::string uid;
	bool enabled = true;
	GameObject* owner;
};

