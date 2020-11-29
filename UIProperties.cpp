#include "UIProperties.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTexture.h"
#include "Model.h"
#include "imgui.h"


UIProperties::UIProperties()
{
}

UIProperties::~UIProperties()
{
}

void UIProperties::Init()
{
}

void UIProperties::Draw()
{
    Model* model = App->renderer->GetModel();

    if (!ImGui::Begin("Properties"))
    {
        ImGui::End();
        return;
    }

    if (model != nullptr) {
        if (ImGui::CollapsingHeader("Transformation")) 
        {

        }

	    if (ImGui::CollapsingHeader("Geometry")) 
        {
            ImGui::Text("Vertices number: %d", model->GetNumVertices());
            ImGui::Text("Indices number: %d", model->GetNumIndices());
            ImGui::Text("Mesehes number: %d", model->GetNumMeshes());
	    }

	    if (ImGui::CollapsingHeader("Texture")) 
        {
            ImGui::Text("Texture preview");
            ImGui::Text("");
            ImGui::Image((void*) model->GetFirstTexture(), ImVec2(200, 200));
            int w, h;
            model->GetFirstTextureSize(w, h);

            ImGui::Text("Width: %d", w);
            ImGui::Text("Height: %d", h);

	    }
    }
    else {
        ImGui::Text("Model is not currently available");
    }


    ImGui::End();
}
