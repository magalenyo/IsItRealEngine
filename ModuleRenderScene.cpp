#include "ModuleRenderScene.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

update_status ModuleRenderScene::Update()
{
    if (App->scene->GetRootNode() != nullptr) {
        RenderGameObjectsRecursively(App->scene->GetRootNode());
    }
    return UPDATE_CONTINUE;
}

void ModuleRenderScene::RenderGameObjectsRecursively(const GameObject* node) const
{
    std::vector<ComponentMaterial*> materials = node->GetComponents<ComponentMaterial>();
    ComponentTransform* transform = node->GetComponent<ComponentTransform>();
    for (ComponentMesh* componentMesh : node->GetComponents<ComponentMesh>()) {
        componentMesh->Draw(materials, transform->GetGlobalModelMatrix());
    }

    for (GameObject* child : node->GetChildren()) {
        RenderGameObjectsRecursively(child);
    }
} 