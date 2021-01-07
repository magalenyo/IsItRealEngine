#include "Component.h"
#include "GUIDGenerator.h"

Component::Component() : uid(GenerateUID()) {}

Component::Component(GameObject* owner, ComponentType type) : uid(GenerateUID()), owner(owner), type(type) {}

Component::~Component()
{
}
