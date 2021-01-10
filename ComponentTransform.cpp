#include "ComponentTransform.h"
#include "Component.h"
#include "GUIDGenerator.h"
#include "GameObject.h"
#include "Math/float4x4.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation, GameObject* owner) : position(position), scale(scale), rotation(rotation), localMatrix(float4x4::FromTRS(position, rotation, scale)), Component(owner, ComponentType::TRANSFORM) {}

void ComponentTransform::CalculateGlobalMatrix(const GameObject* parent)
{
    if (parent != nullptr) {
        ComponentTransform* parentTransform = parent->GetComponent<ComponentTransform>();
        if (parentTransform != nullptr) {
            globalMatrix = localMatrix * parentTransform->globalMatrix;
        }
        else {
            globalMatrix = localMatrix;
        }
    }
    else {
        globalMatrix = localMatrix;
    }
}
