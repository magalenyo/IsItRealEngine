#include "ComponentTransform.h"
#include "Component.h"
#include "GUIDGenerator.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation, GameObject* owner) : position(position), scale(scale), rotation(rotation), Component(owner, ComponentType::TRANSFORM) {}