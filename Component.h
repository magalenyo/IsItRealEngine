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
	virtual void RenderToEditor() {}

	void SetParent(GameObject* parent);
	
	std::string GetUID() const { return uid; };

private:
	std::string uid;
	ComponentType type = ComponentType::UNDEFINED;
	
protected:
	bool enabled = true;
	GameObject* owner = nullptr;
};

