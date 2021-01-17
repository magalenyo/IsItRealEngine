#include "ComponentTransform.h"
#include "Component.h"
#include "Application.h"
#include "ModuleInput.h"
#include "GUIDGenerator.h"
#include "GameObject.h"
#include "Math/float4x4.h"
#include "imgui.h"
#include "Globals.h"

#include "MemoryLeakDetector.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation, GameObject* owner) : position(position), scale(scale), rotation(rotation), localMatrix(float4x4::FromTRS(position, rotation, scale)), globalMatrix(localMatrix), Component(owner, ComponentType::TRANSFORM) {}

void ComponentTransform::RenderToEditor()
{
    if (App->input->GetKey(SDL_SCANCODE_T) == KeyState::KEY_REPEAT)
    {
        current_guizmo_operation = ImGuizmo::TRANSLATE;
    }
    if (App->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_REPEAT)
    {
        current_guizmo_operation = ImGuizmo::ROTATE;
    }
    if (App->input->GetKey(SDL_SCANCODE_Y) == KeyState::KEY_REPEAT)
    {
        current_guizmo_operation = ImGuizmo::SCALE;
    }
    ImGui::PushID(GetUID().c_str());
    ImGui::Text("Transform component");
    if (ImGui::CollapsingHeader("Transform"))
    {
        if (ImGui::RadioButton("Translate", current_guizmo_operation == ImGuizmo::TRANSLATE))
        {
            current_guizmo_operation = ImGuizmo::TRANSLATE;
        }        
        if (ImGui::RadioButton("Rotate", current_guizmo_operation == ImGuizmo::ROTATE))
        {
            current_guizmo_operation = ImGuizmo::ROTATE;
        }
        
        if (ImGui::RadioButton("Scale", current_guizmo_operation == ImGuizmo::SCALE))
        {
            current_guizmo_operation = ImGuizmo::SCALE;
        }
        ImGui::Separator();

        bool changed;
        ImGui::Text("Position");
        ImGui::PushID("Tx"); changed = ImGui::DragFloat("x", &position.x, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Ty"); changed |= ImGui::DragFloat("y", &position.y, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        ImGui::PushID("Tz"); changed |= ImGui::DragFloat("z", &position.z, 0.005f, -FLT_MAX, FLT_MAX, "%.3f"); ImGui::PopID();
        if (ImGui::Button("Reset Position")) 
        {
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
        if (ImGui::Button("Reset Scale")) 
        {
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
        if (ImGui::Button("Reset Rotation")) 
        {
            euler.x = 0;
            euler.y = 0;
            euler.z = 0;
            changed = true;
        }
        euler = euler * DEGTORAD;
        rotation.Set(rotation.FromEulerXYZ(euler.x, euler.y, euler.z));
        ImGui::Text("");

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
    if (owner != nullptr) 
    {
        ComponentTransform* parentTransform = owner->GetParent()->GetComponent<ComponentTransform>();
        if (parentTransform != nullptr) 
        {
            globalMatrix = parentTransform->globalMatrix * localMatrix;
        }
        else 
        {
            globalMatrix = localMatrix;
        }
    }
    else 
    {
        globalMatrix = localMatrix;
    }
}

void ComponentTransform::RegenerateGlobalMatrix()
{
    CalculateGlobalMatrix();
    if (owner != nullptr) 
    {
        for (GameObject* gameObject : owner->GetChildren())
        {
            gameObject->GetComponent<ComponentTransform>()->RegenerateGlobalMatrix();
        }
    }
}

void ComponentTransform::Serialize(Value& value, Document::AllocatorType& allocator)
{
    Component::Serialize(value, allocator);
    Value positionSerialized(kArrayType);
    positionSerialized.PushBack(position.x, allocator);
    positionSerialized.PushBack(position.y, allocator);
    positionSerialized.PushBack(position.z, allocator);
    value.AddMember("position", positionSerialized, allocator);

    Value scaleSerialized(kArrayType);
    scaleSerialized.PushBack(scale.x, allocator);
    scaleSerialized.PushBack(scale.y, allocator);
    scaleSerialized.PushBack(scale.z, allocator);
    value.AddMember("scale", scaleSerialized, allocator);

    Value rotationSerialized(kArrayType);
    rotationSerialized.PushBack(rotation.x, allocator);
    rotationSerialized.PushBack(rotation.y, allocator);
    rotationSerialized.PushBack(rotation.z, allocator);
    rotationSerialized.PushBack(rotation.w, allocator);
    value.AddMember("rotation", rotationSerialized, allocator);

    Value globalMatrixSerialized(kArrayType);

    float3 globalPosition;
    Quat globalRotation;
    float3 globalScale;
    globalMatrix.Decompose(globalPosition, globalRotation, globalScale);
    globalMatrixSerialized.PushBack(globalPosition.x, allocator);
    globalMatrixSerialized.PushBack(globalPosition.y, allocator);
    globalMatrixSerialized.PushBack(globalPosition.z, allocator);

    globalMatrixSerialized.PushBack(globalScale.x, allocator);
    globalMatrixSerialized.PushBack(globalScale.y, allocator);
    globalMatrixSerialized.PushBack(globalScale.z, allocator);

    globalMatrixSerialized.PushBack(globalRotation.x, allocator);
    globalMatrixSerialized.PushBack(globalRotation.y, allocator);
    globalMatrixSerialized.PushBack(globalRotation.z, allocator);
    globalMatrixSerialized.PushBack(globalRotation.w, allocator);

    value.AddMember("globalMatrix", globalMatrixSerialized, allocator);

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