#include "UIConfiguration.h"
#include "imgui.h"
#include "Application.h"
#include "GL/glew.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "Model.h"

#include "MemoryLeakDetector.h"
#include "IL/il.h"

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

        int devilVersion = (int) ilGetInteger(IL_VERSION_NUM);
        char devilVersionFormatted[30];
        sprintf_s(devilVersionFormatted, 30, "DevIL version: %d.%d.%d", devilVersion/100, devilVersion/10%10, devilVersion%10);
        ImGui::Text(devilVersionFormatted);

        int sdlglMajorVersion;
        int sdlglMinorVersion;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlglMajorVersion);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlglMinorVersion);
        ImGui::Text((std::string("GL Version: ") + std::to_string(sdlglMajorVersion) + "." + std::to_string(sdlglMinorVersion)).c_str());

        std::string glewVersion = std::string((char*) glewGetString(GLEW_VERSION));
        ImGui::Text((std::string("GLEW Version: " + glewVersion).c_str()));

        SDL_version compiled;
        SDL_version linked;
        SDL_VERSION(&compiled);
        SDL_GetVersion(&linked);
        char sdlVersion[50];
        sprintf_s(sdlVersion, 50, "Compiled against SDL version %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
        ImGui::Text(sdlVersion);
        sprintf_s(sdlVersion, 50, "Linked against SDL version %d.%d.%d\n", linked.major, linked.minor, linked.patch);
        ImGui::Text(sdlVersion);

        ImGui::Separator();

        std::string glVendor = std::string((char*) glGetString(GL_VENDOR));
        ImGui::Text((std::string("Vendor: " + glVendor).c_str()));

        std::string glRenderer = std::string((char*) glGetString(GL_RENDERER));
        ImGui::Text((std::string("Renderer: " + glRenderer).c_str()));

        std::string glSupported = std::string((char*)glGetString(GL_VERSION));
        ImGui::Text((std::string("OpenGL version supported: " + glSupported).c_str()));

        std::string glsl = std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::Text((std::string("GLSL: " + glsl).c_str()));

        ImGui::Separator();

        int systemRam = round( (SDL_GetSystemRAM() / (float) 1024));
        ImGui::Text((std::string("System RAM: " + std::to_string(systemRam) + "GB").c_str()));

        int cpuCores = SDL_GetCPUCount();
        ImGui::Text((std::string("CPU Cores: " + std::to_string(cpuCores)).c_str()));

        int cacheSize = SDL_GetCPUCacheLineSize();
        ImGui::Text((std::string("L1 Cache Size: " + std::to_string(cacheSize) + "Byte").c_str()));

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

        ImGui::Separator();

        ImGui::Text("Textures location paths"); 
        ImGui::SameLine(); HelpMarker("When loading the Texture, it will first try the default path. If not found, then will try in the default path. If not found, then will try in the textures path.");
        ImGui::Text("");
        ImGui::Text("Default: same directory as .exe");
        ImGui::Text((std::string("Models path: ") + Model::PATH_MODELS).c_str());
        ImGui::Text((std::string("Textures path: ") + Model::PATH_TEXTURES).c_str());
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

