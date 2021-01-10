#pragma once

#include <string>
#include <vector>
#include <typeinfo> 

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, GameObject* parent);
	~GameObject();

	void Update();

	void AddComponent(Component *component);
	void AddGameObject(GameObject *gameObject);
	void RemoveChild(GameObject* child);
	void SetParent(GameObject* gameObject);
	void Reparent(GameObject* newParent);

	bool HasComponents() const;
	bool IsLeaf() const;

	std::string GetName() const;
	std::vector<GameObject*> GetChildren() const;

    template <class T> 
	T* GetComponent() const;

    template <class T> 
	std::vector<T*> GetComponents() const;

private:
	std::string uid;
	std::string name = "Default name";
	GameObject* parent = nullptr;

	std::vector<Component*> components;
	std::vector<GameObject*> children;
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

