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
	virtual ~Component();

	virtual void Enable() {}
	virtual void Update() {}
	virtual void Disable() {}
	virtual void RenderToEditor() {}

	void SetParent(GameObject* parent);
	
	std::string GetUID() const { return uid; };

private:
	std::string uid;
	ComponentType type = ComponentType::UNDEFINED;

protected:
	bool enabled = true;
	GameObject* owner;
};

