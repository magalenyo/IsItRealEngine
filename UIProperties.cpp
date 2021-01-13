#include "UIProperties.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "imgui.h"
//#include <string>


#include "MemoryLeakDetector.h"


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
    if (!ImGui::Begin("Inspector"))
    {
        ImGui::End();
        return;
    }

    GameObject* gameObject = App->editor->GetSelectedGameObject();

    if (gameObject != nullptr) {
        gameObject->RenderToEditor();
        

   //     if (ImGui::CollapsingHeader("Transformation")) 
   //     {
   //         ImGui::Text("Position");
   //         /*ComponentTransform modelTransform = model->GetTransformation();
   //         float3 modelPosition = modelTransform.GetPosition();
   //         ImGui::Text("X: %.1f", modelPosition.x); ImGui::SameLine();
   //         ImGui::Text("Y: %.1f", modelPosition.y); ImGui::SameLine();
   //         ImGui::Text("Z: %.1f", modelPosition.z);*/

   //         ImGui::Text("");

   //         ImGui::Text("Rotation");
   //         // TODO 
   //         /*float3 modelRotation = modelTransform.GetRotation();
   //         ImGui::Text("X: %.1f", modelRotation.x); ImGui::SameLine();
   //         ImGui::Text("Y: %.1f", modelRotation.y); ImGui::SameLine();
   //         ImGui::Text("Z: %.1f", modelRotation.z);*/

			//ImGui::Text("");

			//ImGui::Text("Scale"); 
   //         /*float3 modelScale = modelTransform.GetScale();
   //         ImGui::Text("X: %.1f", modelScale.x); ImGui::SameLine();
   //         ImGui::Text("Y: %.1f", modelScale.y); ImGui::SameLine();
   //         ImGui::Text("Z: %.1f", modelScale.z);
   //         ImGui::Text("");*/
   //     }

	  //  if (ImGui::CollapsingHeader("Geometry")) 
   //     {
   //         ImGui::Text("Number of vertices: %d", model->GetNumVertices());
   //         ImGui::Text("Number of indices: %d", model->GetNumIndices());
   //         ImGui::Text("Number of meshes: %d", model->GetNumMeshes());
   //         ImGui::Text("");
	  //  }

	  //  if (ImGui::CollapsingHeader("Texture")) 
   //     {
   //         ImGui::Text("Texture preview");
   //         int w, h;
   //         model->GetLastTextureSize(w, h);

   //         ImGui::Text("Width: %d", w);
   //         ImGui::Text("Height: %d", h);
   //         ImGui::Text("");
   //         ImGui::Image((void*) model->GetLastTexture(), ImVec2(200, 200));
   //         
   //         ImGui::Text("");
	  //  }
    }
    else {
        ImGui::Text("GameObject is not currently available");
    }


    ImGui::End();
}
