#include "UIConfiguration.h"
#include "Application.h"
#include "GL/glew.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleScene.h"
#include "Model.h"

#include "MemoryLeakDetector.h"
#include "IL/il.h"
#include "assimp/version.h"

#include "Math/float4.h"

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

UIConfiguration::UIConfiguration()
{
}

UIConfiguration::~UIConfiguration()
{
    frames.clear();
    millis.clear();
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
        ImGui::Text("Performance");
		char title[55];
        sprintf_s(title, 55, "Framerate %.1f FPS", fps);
        ImGui::PlotHistogram("##Framerate", &frames[0], frames.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100), sizeof(float));
        sprintf_s(title, 55, "Milliseconds %0.3f ms/frame", frameMillis);
        ImGui::PlotHistogram("##Milliseconds", &millis[0], millis.size(), 0, title, 0.0f, 50.0f, ImVec2(310, 100), sizeof(float));

        ImGui::Separator();
    }

    if (ImGui::CollapsingHeader("Hardware Information")) {
        ImGui::Text("Number of CPUs:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%i", SDL_GetCPUCount());
        ImGui::Text("RAM Memory:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%.1f Gb", SDL_GetSystemRAM() / 1000.0f);
        ImGui::Text("GPU Vendor:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%s", (const char*)glGetString(GL_VENDOR));
        ImGui::Text("GPU Model:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%s", (const char*)glGetString(GL_RENDERER));
        ImGui::Text("GPU OpenGL Version:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%s", (const char*)glGetString(GL_VERSION));
        ImGui::Text("VRAM Available:");
        ImGui::SameLine();
        int vramAvailable;
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vramAvailable);
        ImGui::TextColored(purple, "%.1f Mb", vramAvailable / 1000.0f);
        ImGui::Text("VRAM Budget:");
        ImGui::SameLine();
        int vramBudget;
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vramBudget);
        ImGui::TextColored(purple, "%.1f Mb", vramBudget / 1000.0f);
        ImGui::Text("VRAM Usage:");
        ImGui::SameLine();
        int vramUsage = vramBudget - vramAvailable;
        ImGui::TextColored(purple, "%.1f Mb", vramUsage / 1000.0f);
        ImGui::Text("L1 Cache Size:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%iByte", SDL_GetCPUCacheLineSize());
    }

    if (ImGui::CollapsingHeader("Libraries' Versions")) {
        ImGui::Text("Assimp:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
        ImGui::Text("DevIL:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%i.%i.%i", IL_VERSION / 100, (IL_VERSION % 100) / 10, IL_VERSION & 10);
        int sdlglMajorVersion;
        int sdlglMinorVersion;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlglMajorVersion);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlglMinorVersion);
        ImGui::Text("GL:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%i.%i", sdlglMajorVersion, sdlglMinorVersion);
        ImGui::Text("GLEW:");
        ImGui::SameLine();
        ImGui::TextColored(purple, "%s", glewGetString(GLEW_VERSION));
        ImGui::Text("SDL:");
        ImGui::SameLine();
        SDL_version version;
        SDL_VERSION(&version);
        ImGui::TextColored(purple, "%i.%i.%i", version.major, version.minor, version.patch);
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

        bool& cullFaceActivated = App->renderer->GetGLCullFaceState();
        ImGui::Checkbox("Activate Cull Face", &cullFaceActivated);

        bool& depthTestActivated = App->renderer->GetGLDepthTestState();
        ImGui::Checkbox("Activate Depth Test", &depthTestActivated);
    }

    if (ImGui::CollapsingHeader("Window"))
    {
        // WINDOW MODE
        ImGui::Text("Window mode");
        int currentType = App->window->GetWindowType();
        int type = currentType;
        ImGui::RadioButton("Fullscreen", &type, 0); ImGui::SameLine();
        ImGui::RadioButton("Fullscreen Desktop", &type, 1); ImGui::SameLine();
        ImGui::RadioButton("Borderless", &type, 2); ImGui::SameLine();
        ImGui::RadioButton("Resizable", &type, 3);
        if (currentType != type) {
            App->window->SetWindowType(type);
        }
        ImGui::Separator();

        // WINDOW SIZE
        ImGui::Text("Window size");
        int currentWidth;
        int currentHeight;
        App->window->GetWindowSize(currentWidth, currentHeight);
        if (type != 0 && type != 1) {            // can bug the application because it's true desktop size related to app's size so if changed too fast can lead to problems
            int maxWidth;
            int maxHeight;
            App->window->GetWindowMaxSize(maxWidth, maxHeight);

            int width = currentWidth;
            int height = currentHeight;
            ImGui::SliderInt("Width", &width, 300, maxWidth);
            ImGui::SliderInt("Height", &height, 450, maxHeight);
            
            if (width != currentWidth || height != currentHeight) {
                App->window->SetWindowSize(width, height);
                App->camera->OnWindowResized(width, height);
            }
            
        }
        else {
            ImGui::Text((std::string("Width: ") + std::to_string(currentWidth)).c_str());
            ImGui::Text((std::string("Height: ") + std::to_string(currentHeight)).c_str());
        }
    }

    if (ImGui::CollapsingHeader("Texture"))
    {
        ImGui::Text("Texture filters");
        ImGui::Text("");

        ImGui::Text("Magnification filter");
        int currentMagnificationFilter = App->textures->GetMagnificationFilter();
        int magnificationFilter = App->textures->GetMagnificationFilter();
        ImGui::RadioButton("Magnification GL_LINEAR", &magnificationFilter, GL_LINEAR); ImGui::SameLine();
        ImGui::RadioButton("Magnification GL_NEAREST", &magnificationFilter, GL_NEAREST);
        if (currentMagnificationFilter != magnificationFilter) {
            App->textures->SetMagnificationFilter(magnificationFilter);
        }

        ImGui::Text("Minification filter");
        int currentMinificationFilter = App->textures->GetMinificationFilter();
        int minificationFilter = App->textures->GetMinificationFilter();
        ImGui::RadioButton("Minification GL_LINEAR", &minificationFilter, GL_LINEAR); ImGui::SameLine();
        ImGui::RadioButton("Minification GL_NEAREST", &minificationFilter, GL_NEAREST);
        if (currentMinificationFilter != minificationFilter) {
            App->textures->SetMinificationFilter(minificationFilter);
        }

        if (ImGui::Button("Reset to default filters")) {
            App->textures->SetDefaultConfig();
        }

        ImGui::Separator();

        ImGui::Text("Textures location paths"); 
        ImGui::SameLine(); HelpMarker("When loading the Texture, it will first try the default path. If not found, then will try in the default path. If not found, then will try in the textures path.");
        ImGui::Text("");
        ImGui::Text("Default: same directory as .exe");
        ImGui::Text((std::string("Models path: ") + App->scene->PATH_MODELS).c_str());
        ImGui::Text((std::string("Textures path: ") + App->scene->PATH_TEXTURES).c_str());
    }

    if (ImGui::CollapsingHeader("Input"))
    {
        // These functions were taken from imgui_demo
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse position: (%g, %g)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse position: <INVALID>");

        ImGui::Text("Mouse down:");
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
            if (io.MouseDownDuration[i] >= 0.0f) {
                ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); 
            }
        }

        ImGui::Text("Mouse clicked:");
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
            if (ImGui::IsMouseClicked(i)) { 
                ImGui::SameLine(); ImGui::Text("b%d", i); 
            }
        }

        ImGui::Text("Mouse released:"); 
        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) {
            if (ImGui::IsMouseReleased(i)) { 
                ImGui::SameLine(); ImGui::Text("b%d", i);
            }
        }
    }

    if (ImGui::CollapsingHeader("Camera")) {
        vec front = App->camera->GetFront();
        vec up = App->camera->GetUp();
        vec position = App->camera->GetPosition();
        float nearPlane = App->camera->GetNearPlane();
        float farPlane = App->camera->GetFarPlane();
        float FOV = App->camera->GetFOV();
        float aspectRatio = App->camera->GetAspectRatio();
        float movementSpeed = App->camera->GetMovementSpeed();
        float rotationSpeed = App->camera->GetRotationSpeed();
        float zoomSpeed = App->camera->GetZoomSpeed();
        ImGui::InputFloat3("Front", front.ptr(), "%.3f");
        ImGui::InputFloat3("Up", up.ptr(), "%.3f");
        ImGui::InputFloat3("Position", position.ptr());
        ImGui::InputFloat("Near Plane", &nearPlane);
        ImGui::InputFloat("Far Plane", &farPlane);
        ImGui::InputFloat("FOV", &FOV);
        ImGui::InputFloat("Aspect Ratio", &aspectRatio);
        ImGui::InputFloat("Movement Speed", &movementSpeed);
        ImGui::InputFloat("Rotation Speed", &rotationSpeed);
        ImGui::InputFloat("Zoom Speed", &zoomSpeed);
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

