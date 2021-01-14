#pragma once

#include <vector>
#include "Geometry/AABB.h"
#include "Geometry/OBB.h"
#include <typeinfo> 
#include "assimp/scene.h"

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, GameObject* parent);
	~GameObject();

	void Update();

	void AddComponent(Component *component);				// Adds component to components list
	void AddGameObject(GameObject *gameObject);				// Adds gameObject to children list
	void RemoveChild(GameObject* child);					// Removes child from children list
	void SetParent(GameObject* newParent);					// Set this.parent to newParent
	void Reparent(GameObject* newParent);					// Removes this gameObject from the current Parent, sets this.parent to newParent, adds this gameObject to newParent's children 
	void RemoveChildFromParent();							// Removes this child from parent's children and set this parent to nullptr
	void MoveUpOnHiearchy();								// PRE: Parent needs to have at least 2 children and this can't be the first one. Result: From parent's children, moves this one position UP in parent->children vector.
	void MoveDownOnHierarchy();								// PRE: Parent needs to have at least 2 children and this can't be the last one. Result: From parent's children, moves this one position DOWN in parent->children vector.
	
	void Draw() const;

	virtual void Enable() { enabled = true; }
	virtual void Disable() { enabled = false; }
	bool IsEnabled() { return enabled; }

	bool HasComponents() const;								// Returns true if components list is not empty
	bool IsLeaf() const;									// Returns true if this gameObject does not have any children
	bool IsFirstChildOfParent() const;						// Returns true if this gameObject is the first in parent's children vector
	bool IsLastChildOfParent() const;						// Returns true if this gameObject is the last in parent's children vector
	std::string GetName() const;							// Returns the name of this GameObject
	std::vector<GameObject*> GetChildren() const;			// Returns the list of Children
	void RenderToEditor();
	GameObject* GetParent();

    template <class T> 
	T* GetComponent() const;

    template <class T> 
	std::vector<T*> GetComponents() const;

	AABB GetAABB() const { return aabb; }
	OBB GetOBB() { return obb; }

private:
	std::string uid;
	std::string name = "Default name";
	GameObject* parent = nullptr;
	bool enabled = true;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	AABB aabb;
	OBB obb;
};

template<class T>
inline T* GameObject::GetComponent() const
{
    for (Component* component : components)
    {
        if (typeid(*component) == typeid(T))
        {
            return (T*) component;
        }
    }	

    return nullptr;
}

template<class T>
inline std::vector<T*> GameObject::GetComponents() const
{
	std::vector<T*> result;

	for (Component* component : components)
	{	
		if (typeid(*component) == typeid(T))
		{
			result.push_back((T*) component);
		}
	}

	return result;
}

