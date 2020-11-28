#include "UIConfiguration.h"
#include "imgui.h"

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
        ImGui::PlotHistogram("Framerate", &frames[0], frames.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100), sizeof(float));
        sprintf_s(title, 55, "Milliseconds %0.3f ms/frame", frameMillis);
        ImGui::PlotHistogram("Milliseconds", &millis[0], millis.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100), sizeof(float));
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

