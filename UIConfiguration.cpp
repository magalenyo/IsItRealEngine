#include "UIConfiguration.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleRender.h"

#include "Math/float4.h"


//#ifndef GImGui
//ImGuiContext* GImGui = NULL;
//#endif

UIConfiguration::UIConfiguration()
{
}

UIConfiguration::~UIConfiguration()
{
}

void UIConfiguration::Draw()
{
    float fps = ImGui::GetIO().Framerate;
    float frameMillis = 1000.0 / fps;
    AddFrame(fps);
    AddMillis(frameMillis);

    if (!ImGui::Begin("Configuration"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("Application"))
    {
        //Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		char title[55];
		
        sprintf_s(title, 25, "Framerate %.1f FPS", fps);
        ImGui::PlotHistogram("##Framerate", &frames[0], frames.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100), sizeof(float));
        sprintf_s(title, 55, "Milliseconds %0.3f ms/frame", frameMillis);
        ImGui::PlotHistogram("##Milliseconds", &millis[0], millis.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100), sizeof(float));
    }
    
    if (ImGui::CollapsingHeader("Renderer"))
    {
        bool &gridActivated = App->renderer->GetGridState();
        ImGui::Checkbox("Activate Grid", &gridActivated);

        if (gridActivated) {
            float3 auxColor = App->renderer->GetGridColor();
            float color[] = { auxColor.x, auxColor.y, auxColor.z };
            ImGui::ColorEdit3("Color", color);
            App->renderer->SetGridColor(float3(color[0], color[1], color[2]));
        }
        
        bool& axisActivated = App->renderer->GetAxisState();
        ImGui::Checkbox("Activate Axis", &axisActivated);

        bool& modelActivated = App->renderer->GetModelState();
        ImGui::Checkbox("Activate Model", &modelActivated);
    }

    ImGui::End();
}

void UIConfiguration::AddFrame(float fps)
{    
    if (frames.size() == MAX_FRAMES_STORAGE) {
        frames.erase(frames.begin());
    }
    frames.push_back(fps);
}

void UIConfiguration::AddMillis(float frameMillis)
{
    if (millis.size() == MAX_FRAMES_STORAGE) {
        millis.erase(millis.begin());
    }
    millis.push_back(frameMillis);
}

