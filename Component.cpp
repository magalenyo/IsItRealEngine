#include "Component.h"
#include "GUIDGenerator.h"

#include "MemoryLeakDetector.h"

Component::Component() : uid(GenerateUID()) {}

Component::Component(GameObject* owner, ComponentType type) : uid(GenerateUID()), owner(owner), type(type) {}

Component::~Component()
{
}

void Component::Serialize(Value& value, Document::AllocatorType& allocator)
{
	value.AddMember("uid", StringRef(uid.c_str()), allocator);
	value.AddMember("enabled", enabled, allocator);
	value.AddMember("type", type, allocator);
}

void Component::SetParent(GameObject* parent)
{
	owner = parent;
}
