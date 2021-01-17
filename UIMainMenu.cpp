#include "UIMainMenu.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "SceneImporter.h"
#include "imgui.h"

#include <windows.h>
#include <shellapi.h>
#include <string>

#include "MemoryLeakDetector.h"

UIMainMenu::UIMainMenu()
{
}

UIMainMenu::~UIMainMenu()
{

}

void UIMainMenu::Draw()
{
    if (show_about) {
        RenderAbout(&show_about);
    }

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    }
    static bool show_app_dockspace = false;
    ImGui::Begin("DockSpace Demo", &show_app_dockspace, window_flags);
    if (!opt_padding)
    {
        ImGui::PopStyleVar();
    }

    if (opt_fullscreen)
    {
        ImGui::PopStyleVar(2);
    }

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save SCENE")) 
            {
                App->sceneImporter->ExportScene(App->scene->GetRootNode());
            }

            if (ImGui::MenuItem("Load SCENE")) 
            {
                App->sceneImporter->ImportScene(App->sceneImporter->PATH_ROOT_SCENE);
            }

            if (ImGui::MenuItem("Quit")) 
            {
                App->editor->ExitApplication();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("External links")) 
        {
            if (ImGui::MenuItem("Github")) 
            {
                ShellExecute(0, 0, "https://github.com/magalenyo/IsItRealEngine", 0, 0, SW_SHOW);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About RealDisunity", NULL, &show_about);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            bool& showConfiguration = App->editor->GetShowConfiguration();
            ImGui::MenuItem("Configuration Window", NULL, &showConfiguration);

            bool& showConsole = App->editor->GetShowConsole();
            ImGui::MenuItem("Console Window", NULL, &showConsole);

            bool& showProperties = App->editor->GetShowProperties();
            ImGui::MenuItem("Inspector Window", NULL, &showProperties);

            bool& showSceneView = App->editor->GetShowSceneView();
            ImGui::MenuItem("GameView Window", NULL, &showSceneView);

            bool& showHierarchyView = App->editor->GetShowHierarchyView();
            ImGui::MenuItem("Hierarchy Window", NULL, &showHierarchyView);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void UIMainMenu::RenderAbout(bool* p_open)
{
    if (!ImGui::Begin("About RealDisunity", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
   
    ImGui::Text("RealDisunity v1.0\n");
    ImGui::Text("Engine developed as the Second Assignment for the UPC's Master Degree in Advanced Programming for AAA Video Games.");
    ImGui::Separator();
    ImGui::Text("Authors: Miguel Ángel Bueno Rivera - magalenyo on GitHub");
    ImGui::Text("         Isaac Fernández Fontao - isaacfdez on GitHub");
    ImGui::Text("Github Repository: https://github.com/magalenyo/IsItRealEngine");
    ImGui::Text("");

    if (ImGui::CollapsingHeader("MIT License"))
    {

        std::string aux =
            std::string("MIT License\n\n") +

            "Copyright(c) 2020 magalenyo, isaacfdez\n\n" +

            "Permission is hereby granted, free of charge, to any person obtaining a copy\n" +
            "of this software and associated documentation files(the \"Software\"), to deal\n" +
            "in the Software without restriction, including without limitation the rights\n" +
            "to use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n" +
            "copies of the Software, and to permit persons to whom the Software is\n" +
            "furnished to do so, subject to the following conditions :\n\n" +

            "The above copyright noticeand this permission notice shall be included in all\n" +
            "copies or substantial portions of the Software. \n" +
            "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR \n" +
            "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n" +
            "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE \n" +
            "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \n" +
            "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n" +
            "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n" +
            "SOFTWARE.\n";
        
        ImGui::Text(aux.c_str());
    }

    ImGui::End();
}


