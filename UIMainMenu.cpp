#include "UIMainMenu.h"
#include "Application.h"
#include "ModuleEditor.h"
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

    if (show_license) {
        RenderLicense(&show_license);
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::Button("Github Page")) {
            ShellExecute(0, 0, "https://github.com/magalenyo/IsItRealEngine", 0, 0, SW_SHOW);
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("About IsItRealEngine", NULL, &show_about);
            ImGui::Separator();
            ImGui::MenuItem("License", NULL, &show_license);
            // TODO MAYBE?
            //ImGui::MenuItem("Manual", NULL, &show_about);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            bool& showConfiguration = App->editor->GetShowConfiguration();
            ImGui::MenuItem("Configuration Window", NULL, &showConfiguration);

            bool& showConsole = App->editor->GetShowConsole();
            ImGui::MenuItem("Console Window", NULL, &showConsole);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Quit"))
        {
            App->editor->ExitApplication();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void UIMainMenu::RenderAbout(bool* p_open)
{
    if (!ImGui::Begin("About IsItRealEngine", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }
   
    ImGui::Text("IsItRealEngine v1.0\n");
    ImGui::Text("Engine developed as First Assignment for the UPC's Master Degree in Advanced Programming for AAA Video Games.");
    ImGui::Separator();
    ImGui::Text("Author: Miguel Ángel Bueno Rivera - magalenyo on GitHub");
    ImGui::Text("Github Repository: https://github.com/magalenyo/IsItRealEngine");
    ImGui::Text("");

    if (ImGui::CollapsingHeader("MIT License"))
    {

        std::string aux =
            std::string("MIT License\n\n") +

            "Copyright(c) 2020 magalenyo\n\n" +

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

void UIMainMenu::RenderLicense(bool* p_open)
{
    if (!ImGui::Begin("License", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    std::string aux =
        std::string("MIT License\n\n") +

        "Copyright(c) 2020 magalenyo\n\n" +

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

    ImGui::End();
}


