#pragma once

#include <string>
#include "FSJsonIncluders.h"

class GameObject;

class Component
{
public:
	enum ComponentType 
	{
		TRANSFORM,
		MESH,
		MATERIAL,
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
	virtual void Serialize(Value& value, Document::AllocatorType& allocator);
	virtual Component* Deserialize() { return new Component(); }

	void SetParent(GameObject* parent);
	
	std::string GetUID() const { return uid; };
	bool IsEnabled() const { return enabled; }

protected:
	std::string uid;
	ComponentType type		= ComponentType::UNDEFINED;
	bool enabled			= true;
	GameObject* owner		= nullptr;
};

