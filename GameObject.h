#pragma once

#include <string>
#include <vector>

class Component;

class GameObject
{
public:
	void Update();

private:
	std::string name;
	std::vector<Component*> components;
	GameObject* parent;
	std::vector<GameObject*> children;
};

