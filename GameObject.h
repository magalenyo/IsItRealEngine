#pragma once

#include <string>
#include <vector>

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name, GameObject* parent);

	void Update();

	void AddComponent(const Component &component);
	void AddGameObject(const GameObject &gameObject);
	void SetParent(const GameObject& gameObject);

private:
	std::string uid;
	std::string name = "Default name";
	GameObject* parent = nullptr;

	std::vector<Component*> components;
	std::vector<GameObject*> children;
};

