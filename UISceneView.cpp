#include "UISceneView.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "MemoryLeakDetector.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ImGuizmo.h"


void UISceneView::Draw()
{

    ImGui::Begin("GameWindow");
    {
        int imguizmoSize = 100;
        ImGui::BeginChild("GameRender");
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();

        //Center Game Buttons
        //ImGui::SameLine((wsize.x / 2) - 100);
        //if (ImGui::Button("Play"))
        //{

        //}
        //ImGui::SameLine();
        //if (ImGui::Button("Pause"))
        //{

        //}
        //ImGui::SameLine();
        //if (ImGui::Button("Stop"))
        //{

        //}
        //ImGui::SameLine();
        //if (ImGui::Button("Step"))
        //{

        //}

        if (App->renderer->viewportWidth != wsize.x || App->renderer->viewportHeight != wsize.y) 
        {
            App->renderer->OnSceneResize(wsize.x, wsize.y);
            App->camera->OnWindowResized(wsize.x, wsize.y);

            framebuffer_size = { wsize.x, wsize.y };
        }

        mousePos = ImGui::GetMousePos();
        viewportPos = ImGui::GetCursorScreenPos();

        ImGui::Image((ImTextureID)App->renderer->GetSceneTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

        GameObject* picked = App->editor->GetSelectedGameObject();

        if (picked != nullptr)
        {
            float viewManipulateRight = viewportPos.x + framebuffer_size.x;
            float viewManipulateTop = viewportPos.y;
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(viewportPos.x, viewportPos.y, framebuffer_size.x, framebuffer_size.y);

            Frustum& default_frustum = App->camera->GetCamera();
            float4x4 camera_view = float4x4(default_frustum.ViewMatrix()).Transposed();
            float4x4 camera_projection = default_frustum.ProjectionMatrix().Transposed();

            ComponentTransform* transform = picked->GetComponent<ComponentTransform>();
            if (transform != nullptr)
            {
                transform->CalculateGlobalMatrix();
                float4x4 matrix = transform->GetLocalMatrix().Transposed();

                if (ImGuizmo::Manipulate(camera_view.ptr(), camera_projection.ptr(), transform->GetGizmoOperation(), transform->GetGizmoMode(), matrix.ptr(), NULL, NULL))
                {
                    float4x4 matrix_t = matrix.Transposed();
                    float3 angles = matrix_t.ToEulerXYZ();
                    transform->RecalculateMatrices(matrix_t.TranslatePart(), Quat::FromEulerXYZ(angles[0], angles[1], angles[2]), matrix_t.GetScale());
                }
                ImGuizmo::ViewManipulate(camera_view.ptr(), 4, ImVec2(viewManipulateRight - imguizmoSize, viewManipulateTop), ImVec2(imguizmoSize, imguizmoSize), 0x10101010);
            }
        }

        ImGui::EndChild();
    }
    ImGui::End();
}

float2 UISceneView::GetMousePos() const
{
    return float2(mousePos.x, mousePos.y);
}

float2 UISceneView::GetViewportPos() const
{
    return float2(viewportPos.x, viewportPos.y);
}
