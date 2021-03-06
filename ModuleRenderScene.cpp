#include "ModuleRenderScene.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "Quadtree.h"

update_status ModuleRenderScene::Update()
{
    if (App->scene->GetRootNode() != nullptr) 
    {
        RenderGameObjectsRecursively(App->scene->GetRootNode());
    }
    return UPDATE_CONTINUE;
}

void ModuleRenderScene::RenderGameObjectsRecursively(const GameObject* node) const
{
    GameObject* camera = App->scene->GetCamera();
    ComponentCamera* cCamera = camera->GetComponent<ComponentCamera>();

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
        else
        {
            node->Draw();
        }
    }

    camera->Draw();

    for (GameObject* child : node->GetChildren()) 
    {
        RenderGameObjectsRecursively(child);
    }
}

std::vector<GameObject*> ModuleRenderScene::TestCulling(const std::vector<GameObject*> candidatesToCull, Frustum frustum) const
{
    std::vector<GameObject*> intersectingObjects;

    for (unsigned int i = 0; i < candidatesToCull.size(); ++i)
    {
        if (frustum.Intersects(candidatesToCull[i]->GetOBB()))
        {
            intersectingObjects.push_back(candidatesToCull[i]);
        }
    }
    return intersectingObjects;
}
