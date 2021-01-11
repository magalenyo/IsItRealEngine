#include "ComponentTransform.h"
#include "Component.h"
#include "GUIDGenerator.h"
#include "GameObject.h"
#include "Math/float4x4.h"
#include "imgui.h"
#include "Globals.h"

#include "MemoryLeakDetector.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation, GameObject* owner) : position(position), scale(scale), rotation(rotation), localMatrix(float4x4::FromTRS(position, rotation, scale)), Component(owner, ComponentType::TRANSFORM) {}

void ComponentTransform::RenderToEditor()
{
    ImGui::PushID(GetUID().c_str());
    ImGui::Checkbox("Transform component", &enabled);
    if (ImGui::CollapsingHeader("Transform"))
    {
        ImGui::Text("Position");
        ImGui::PushID("Tx"); ImGui::DragFloat("x", &position.x, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Ty"); ImGui::DragFloat("y", &position.y, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Tz"); ImGui::DragFloat("z", &position.z, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();

        ImGui::Text("Scale");
        ImGui::PushID("Sx"); ImGui::DragFloat("x", &scale.x, 0.005f, 0, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Sy"); ImGui::DragFloat("y", &scale.y, 0.005f, 0, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Sz"); ImGui::DragFloat("z", &scale.z, 0.005f, 0, FLT_MAX, "%.3f"); ImGui::PopID();

        ImGui::Text("Rotation");
        float3 euler = rotation.ToEulerXYZ() * RADTODEG;
        ImGui::PushID("Rx"); ImGui::DragFloat("x", &euler.x, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Ry"); ImGui::DragFloat("y", &euler.y, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Rz"); ImGui::DragFloat("z", &euler.z, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        euler = euler * DEGTORAD;
        rotation.Set(rotation.FromEulerXYZ(euler.x, euler.y, euler.z));

        CalculateGlobalMatrix(owner);
    }

    ImGui::Separator();
    ImGui::PopID();
}

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

void ComponentTransform::RegenerateGlobalMatrix()
{
    
}
