#include "GameObject.h"
#include "GUIDGenerator.h"

GameObject::GameObject() : uid(GenerateUID()) {}

GameObject::GameObject(const std::string& name, GameObject* parent) : name(name), parent(parent), uid(GenerateUID()) {}
