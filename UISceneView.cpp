#include "UISceneView.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "MemoryLeakDetector.h"


void UISceneView::Draw()
{

    ImGui::Begin("GameWindow");
    {
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

        if (App->renderer->viewportWidth != wsize.x || App->renderer->viewportHeight != wsize.y) {
            App->renderer->OnSceneResize(wsize.x, wsize.y);
            App->camera->OnWindowResized(wsize.x, wsize.y);
        }
        
        mousePos = ImGui::GetCursorScreenPos();

        ImGui::Image((ImTextureID)App->renderer->GetSceneTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();
}

float2 UISceneView::GetMousePos() const
{
    return float2(mousePos.x, mousePos.y);
}
