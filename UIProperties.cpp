#include "UIProperties.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "imgui.h"
#include <string>

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
    }
    else {
        ImGui::Text("GameObject is not currently available");
    }


    ImGui::End();
}
