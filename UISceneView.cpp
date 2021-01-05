#include "UISceneView.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "Model.h"
#include "imgui.h"

void UISceneView::Draw()
{

    ImGui::Begin("GameWindow");
    {
        // TODO CALL RESIZE IF NEEDED

        // Using a Child allow to fill all the space of the window.
        // It also alows customization
        ImGui::BeginChild("GameRender");
        // Get the size of the child (i.e. the whole draw size of the windows).

        
        ImVec2 wsize = ImGui::GetWindowSize();

        if (App->renderer->viewportWidth != wsize.x || App->renderer->viewportHeight != wsize.y) {
            App->renderer->OnSceneResize(wsize.x, wsize.y);
            App->camera->OnWindowResized(wsize.x, wsize.y);
        }
        
        // Because I use the texture from OpenGL, I need to invert the V from the UV.
        ImGui::Image((ImTextureID)App->renderer->GetSceneTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();
}