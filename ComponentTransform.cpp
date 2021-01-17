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
        if (ImGui::IsKeyPressed(90)) // W key
            current_guizmo_operation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(69)) // E key
            current_guizmo_operation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(82)) // R key
            current_guizmo_operation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", current_guizmo_operation == ImGuizmo::TRANSLATE))
            current_guizmo_operation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", current_guizmo_operation == ImGuizmo::ROTATE))
            current_guizmo_operation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", current_guizmo_operation == ImGuizmo::SCALE))
            current_guizmo_operation = ImGuizmo::SCALE;
        ImGui::Separator();

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

        /*if (current_guizmo_operation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", current_guizmo_mode == ImGuizmo::LOCAL))
                current_guizmo_mode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", current_guizmo_mode == ImGuizmo::WORLD))
                current_guizmo_mode = ImGuizmo::WORLD;
        }
        ImGui::Checkbox("##snap", &useSnap);
        ImGui::SameLine();

        switch (current_guizmo_operation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }*/

        if (changed)
        {
            RecalculateMatrices(position, rotation, scale);
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

void ComponentTransform::RecalculateMatrices(float3 _position, Quat _rotation, float3 _scale)
{
    position = _position;
    rotation = _rotation;
    scale = _scale;
    localMatrix = float4x4::FromTRS(_position, _rotation, _scale);
    RegenerateGlobalMatrix();
}

ImGuizmo::OPERATION ComponentTransform::GetGizmoOperation() const
{
    return current_guizmo_operation;
}

ImGuizmo::MODE ComponentTransform::GetGizmoMode() const
{
    return current_guizmo_mode;
}

bool ComponentTransform::GetUseSnap() const
{
    return useSnap;
}

float3 ComponentTransform::GetSnap()
{
    return float3(snap[0], snap[1], snap[2]);
}
