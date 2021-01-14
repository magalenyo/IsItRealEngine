#include "ModuleRenderScene.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

ModuleRenderScene::ModuleRenderScene()
{
}

ModuleRenderScene::~ModuleRenderScene()
{
}

bool ModuleRenderScene::Init()
{
    return true;
}

update_status ModuleRenderScene::Update()
{
    if (App->scene->GetRootNode() != nullptr) 
    {
        RenderGameObjectsRecursively(App->scene->GetRootNode());
    }
    return UPDATE_CONTINUE;
}

void ModuleRenderScene::RenderGameObjectsRecursively(const GameObject* node)
{
    ComponentCamera* cCamera = App->scene->GetCamera();

    if (cCamera != nullptr)
    {
        if (cCamera->GetCullingStatus())
        {
            std::vector<GameObject*> candidatesToCull = App->scene->GetQuadtree()->GetObjectsCollided(cCamera->GetFrustum());
            std::vector<GameObject*> objectsToCull = TestCulling(candidatesToCull, cCamera->GetFrustum());

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
    }

    node->Draw();
    
    std::vector<GameObject*> children = node->GetChildren();
    
    for (unsigned int i = 0; i < children.size(); ++i)
    {
        RenderGameObjectsRecursively(children[i]);
    }
}

std::vector<GameObject*> ModuleRenderScene::TestCulling(const std::vector<GameObject*> candidatesToCull, Frustum frustum)
{
    std::vector<GameObject*> intersectingObjects;

    for (unsigned int i = 0; i < candidatesToCull.size(); ++i)
    {
        if (frustum.Intersects(candidatesToCull[i]->GetAABB()))
        {
            intersectingObjects.push_back(candidatesToCull[i]);
        }
    }
    return intersectingObjects;
}
