#pragma once

#include <string>
#include "FSJsonIncluders.h"

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
	virtual void Serialize(Value& value, Document::AllocatorType& allocator);

	void SetParent(GameObject* parent);
	
	std::string GetUID() const { return uid; };

protected:
	std::string uid;
	ComponentType type = ComponentType::UNDEFINED;
	bool enabled = true;
	GameObject* owner;
};

