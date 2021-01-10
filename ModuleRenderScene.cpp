#include "ModuleRenderScene.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

ModuleRenderScene::ModuleRenderScene()
{
}

ModuleRenderScene::~ModuleRenderScene()
{
}

bool ModuleRenderScene::Init()
{
}

update_status ModuleRenderScene::Update()
{
    if (App->scene->GetRootNode() != nullptr) 
    {
        if (App->renderer->cullingCamera.IsActive())
        {
            std::vector<GameObject*> candidatesToCull = App->scene->GetQuadtree()->GetObjectsCollided(App->renderer->cullingCamera.GetFrustum());
            std::vector<GameObject*> objectsToCull = TestCulling(candidatesToCull);

            for (unsigned int i = 0; i < objectsToCull.size(); ++i)
            {
                if (objectsToCull[i] != App->scene->GetRootNode())
                {
                    objectsToCull[i]->Draw();
                }
            }
            candidatesToCull.clear();
            objectsToCull.clear();
        }
        else
        {
            RenderGameObjectsRecursively(App->scene->GetRootNode());
        }
    }
    return UPDATE_CONTINUE;
}

void ModuleRenderScene::RenderGameObjectsRecursively(const GameObject* node) const
{
    if (node != App->scene->GetRootNode())
    {
        node->Draw();
    }

    std::vector<GameObject*> children = node->GetChildren();
    
    for (unsigned int i = 0; i < children.size(); ++i)
    {
        RenderGameObjectsRecursively(children[i]);
    }
}

std::vector<GameObject*> ModuleRenderScene::TestCulling(const std::vector<GameObject*> candidatesToCull)
{
    std::vector<GameObject*> intersectingObjects;

    for (unsigned int i = 0; i < candidatesToCull.size(); ++i)
    {
        if (App->renderer->cullingCamera->GetFrustum().Intersects(candidatesToCull[i]->GetAABB()))
        {
            intersectingObjects.push_back(candidatesToCull[i]);
        }
    }
    return intersectingObjects;
}
