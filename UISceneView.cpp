#include "UISceneView.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Model.h"
#include "imgui.h"


void UISceneView::Draw()
{


    //if (ImGui::BeginChild("GameRender")) {
    //    // Get the size of the child (i.e. the whole draw size of the windows).
    //    ImVec2 wsize = ImGui::GetWindowSize();
    //    // Because I use the texture from OpenGL, I need to invert the V from the UV.
    //    ImGui::Image((ImTextureID)App->renderer->GetSceneTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));
    //}

    //ImGui::EndChild();

    ImGui::Begin("GameWindow");
    {
        // TODO CALL RESIZE IF NEEDED

        // Using a Child allow to fill all the space of the window.
        // It also alows customization
        ImGui::BeginChild("GameRender");
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();

        /*if (width != wsize.x || height != wsize.y) {
            App->renderer->OnSceneResize(width, height);
        }*/

        width = wsize.x;
        height = wsize.y;
        
        // Because I use the texture from OpenGL, I need to invert the V from the UV.
        ImGui::Image((ImTextureID)App->renderer->GetSceneTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();
}
