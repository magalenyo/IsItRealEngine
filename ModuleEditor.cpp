#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "UIComponent.h"
#include "UIMainMenu.h"
#include "UIConsole.h"
#include "UIConfiguration.h"
#include "UIProperties.h"
#include "UISceneView.h"
#include "UIHierarchy.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
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
    ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetContext());
    ImGui_ImplOpenGL3_Init(glsl_version);
    isReady = true;

    components.push_back(mainMenu = new UIMainMenu());
    components.push_back(console = new UIConsole());
    components.push_back(configuration = new UIConfiguration());
    components.push_back(properties = new UIProperties());
    components.push_back(scene = new UISceneView());
    components.push_back(hierarchy = new UIHierarchy());

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows     

    ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    };

    ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetContext());
    ImGui_ImplOpenGL3_Init(glsl_version);

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

    mainMenu = nullptr;
    console = nullptr;
    configuration = nullptr;
    properties = nullptr;
    scene = nullptr;
    hierarchy = nullptr;

    return true;
}

update_status ModuleEditor::Update()
{
    if (showMainMenu) {
        mainMenu->Draw();
    }

    if (showConsole) {
        console->Draw();
    }

    if (showConfiguration) {
        configuration->Draw();
    }
    
    if (showProperties) {
        properties->Draw();
    }

    if (showScene) {
        scene->Draw();
    }

    if (showHierarchy) {
        hierarchy->Draw();
    }

    //ImGui::ShowDemoWindow();

    return updateStatus;
}


update_status ModuleEditor::PreUpdate()
{
    // THESE SHOULD BE CALLED BEFORE ANY DRAWING
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Needed for Viewport
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    return UPDATE_CONTINUE;
}

void ModuleEditor::ExitApplication()
{
    LOG("Exiting application from Editor...");
    updateStatus = UPDATE_STOP;
}

bool& ModuleEditor::GetShowConsole()
{
    return showConsole;
}

bool& ModuleEditor::GetShowConfiguration()
{
    return showConfiguration;
}

bool& ModuleEditor::GetShowProperties()
{
    return showProperties;
}

bool& ModuleEditor::GetShowSceneView()
{
    return showScene;
}

bool& ModuleEditor::GetShowHierarchyView()
{
    return showHierarchy;
}

GameObject* ModuleEditor::GetSelectedGameObject()
{
    if (hierarchy != nullptr) 
    {
        return hierarchy->GetSelectedGameObject();
    }
    else return nullptr;
}

void ModuleEditor::SetSelectedGameObject(GameObject* newSelected)
{
    if (hierarchy != nullptr)
    {
        hierarchy->SetSelectedGameObject(newSelected);
    }
}

float2 ModuleEditor::GetSceneMousePosition() const
{
    if (scene != nullptr) {
        return scene->GetMousePos();
    }
    else return float2(0, 0);
}

float2 ModuleEditor::GetSceneViewportPosition() const
{
    if (scene != nullptr) {
        return scene->GetViewportPos();
    }
    else return float2(0, 0);
}

