#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "UIComponent.h"
#include "UIMainMenu.h"
#include "UIConsole.h"
#include "UIConfiguration.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <string>

#include "MemoryLeakDetector.h"

using namespace std;

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    const char* glsl_version = "#version 130";

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->GetContext());
    ImGui_ImplOpenGL3_Init(glsl_version);
    isReady = true;

    components.push_back(mainMenu = new UIMainMenu());
    components.push_back(console = new UIConsole());
    components.push_back(configuration = new UIConfiguration());

    return true;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    for (UIComponent* component : components) {
        delete component;
    }
    components.clear();

    return true;
}

update_status ModuleEditor::Update()
{
    if (show_mainMenu) {
        mainMenu->Draw();
    }

    if (show_console) {
        console->Draw();
    }

    if (show_configuration) {
        configuration->Draw();
    }

    ImGui::ShowDemoWindow();
    return updateStatus;
}


update_status ModuleEditor::PreUpdate()
{
    // THESE SHOULD BE CALLED BEFORE ANY DRAWING
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();
    return UPDATE_CONTINUE;
}


void ModuleEditor::ExitApplication()
{
    LOG("Exiting application from Editor...");
    updateStatus = UPDATE_STOP;
}

bool& ModuleEditor::GetShowConsole()
{
    return show_console;
}

bool& ModuleEditor::GetShowConfiguration()
{
    return show_configuration;
}

