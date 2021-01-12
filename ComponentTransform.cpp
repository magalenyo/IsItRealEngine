#include "ComponentTransform.h"
#include "Component.h"
#include "GUIDGenerator.h"
#include "GameObject.h"
#include "Math/float4x4.h"
#include "imgui.h"
#include "Globals.h"

#include "MemoryLeakDetector.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation, GameObject* owner) : position(position), scale(scale), rotation(rotation), localMatrix(float4x4::FromTRS(position, rotation, scale)), globalMatrix(localMatrix), Component(owner, ComponentType::TRANSFORM) {}

void ComponentTransform::RenderToEditor()
{
    ImGui::PushID(GetUID().c_str());
    ImGui::Text("Transform component");
    if (ImGui::CollapsingHeader("Transform"))
    {
       
        bool changed;
        ImGui::Text("Position");
        ImGui::PushID("Tx"); changed = ImGui::DragFloat("x", &position.x, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Ty"); changed |= ImGui::DragFloat("y", &position.y, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Tz"); changed |= ImGui::DragFloat("z", &position.z, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        if (ImGui::Button("Reset Position")) {
            position.x = 0;
            position.y = 0;
            position.z = 0;
            changed = true;
        }
        ImGui::Text("");

        ImGui::Text("Scale");
        ImGui::PushID("Sx"); changed |= ImGui::DragFloat("x", &scale.x, 0.005f, 0, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Sy"); changed |= ImGui::DragFloat("y", &scale.y, 0.005f, 0, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Sz"); changed |= ImGui::DragFloat("z", &scale.z, 0.005f, 0, FLT_MAX, "%.3f"); ImGui::PopID();
        if (ImGui::Button("Reset Scale")) {
            scale.x = 1;
            scale.y = 1;
            scale.z = 1;
            changed = true;
        }
        ImGui::Text("");

        ImGui::Text("Rotation");
        float3 euler = rotation.ToEulerXYZ() * RADTODEG;
        ImGui::PushID("Rx"); changed |= ImGui::DragFloat("x", &euler.x, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Ry"); changed |= ImGui::DragFloat("y", &euler.y, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Rz"); changed |= ImGui::DragFloat("z", &euler.z, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        if (ImGui::Button("Reset Rotation")) {
            euler.x = 0;
            euler.y = 0;
            euler.z = 0;
            changed = true;
        }
        euler = euler * DEGTORAD;
        rotation.Set(rotation.FromEulerXYZ(euler.x, euler.y, euler.z));
        ImGui::Text("");

        
        if (changed) {
            localMatrix = float4x4::FromTRS(position, rotation, scale);
            RegenerateGlobalMatrix();
        }
    }

    ImGui::Separator();
    ImGui::PopID();
}

void ComponentTransform::CalculateGlobalMatrix()
{
    if (owner != nullptr) {
        ComponentTransform* parentTransform = owner->GetParent()->GetComponent<ComponentTransform>();
        if (parentTransform != nullptr) {
            globalMatrix = parentTransform->globalMatrix * localMatrix;
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
    CalculateGlobalMatrix();
    if (owner != nullptr) {
        for (GameObject* gameObject : owner->GetChildren()) {
            gameObject->GetComponent<ComponentTransform>()->RegenerateGlobalMatrix();
        }
    }
}
