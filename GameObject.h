#pragma once

#include <string>
#include <vector>
#include "Geometry/AABB.h"
#include "Geometry/OBB.h"

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, GameObject* parent);

	void Update();

	void AddComponent(Component *component);
	void AddGameObject(GameObject *gameObject);
	void RemoveChild(GameObject* child);
	void SetParent(GameObject* gameObject);
	void Reparent(GameObject* newParent);
	void Draw() const;

	virtual void Enable() { enabled = true; }
	virtual void Disable() { enabled = false; }
	bool IsEnabled() { return enabled; }

	bool HasComponents() const;
	bool IsLeaf() const;

	std::string GetName() const;
	std::vector<GameObject*> GetChildren() const;

    template <class T> T* GetComponent() const;
    template <class T> std::vector<T*> GetComponents() const;

	AABB GetAABB() { return aabb; }
	OBB GetOBB() { return obb; }

private:
	std::string uid;
	std::string name = "Default name";
	GameObject* parent = nullptr;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	AABB aabb;
	OBB obb;

	bool enabled = true;
};

template<class T>
inline T* GameObject::GetComponent() const
{
    for (Component* component : components)
    {
        if (component->GetType() == T::static_type)
        {
            return (T)component;
        }
    }

    return nullptr;
}

template<class T>
inline std::vector<T*> GameObject::GetComponents() const
{
	std::vector<T*> aux_components;
	
	for (Component* component : components)
	{
	    if (component->GetType() == T::static_type)
	    {
	        aux_components.push_back((T*)component);
	    }
	}
	
	return aux_components;
}