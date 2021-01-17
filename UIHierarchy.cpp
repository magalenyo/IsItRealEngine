#include "UIHierarchy.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "imgui.h"

#include "MemoryLeakDetector.h"

void UIHierarchy::Draw()
{
    //clickedNode = nullptr;
    if (!ImGui::Begin("Hierarchy"))
    {
        ImGui::End();
        return;
    }

    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    RenderRecursively(App->scene->GetRootNode());

    // 'selection_mask' is dumb representation of what may be user-side selection state.
    //  You may retain selection state inside or outside your objects in whatever format you see fit.
    // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
    /// of the loop. May be a pointer to your own node type, etc.
    //static int selection_mask = (1 << 2);
    //int node_clicked = -1;
    //for (int i = 0; i < 6; i++)
    //{
    //    // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
    //    ImGuiTreeNodeFlags node_flags = base_flags;
    //    const bool is_selected = (selection_mask & (1 << i)) != 0;
    //    if (is_selected)
    //        node_flags |= ImGuiTreeNodeFlags_Selected;
    //    if (i < 3)
    //    {
    //        // Items 0..2 are Tree Node
    //        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
    //        if (ImGui::IsItemClicked())
    //            node_clicked = i;
    //        if (ImGui::BeginDragDropSource())
    //        {
    //            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
    //            ImGui::Text("This is a drag and drop source");
    //            ImGui::EndDragDropSource();
    //        }
    //        if (node_open)
    //        {
    //            ImGui::BulletText("Blah blah\nBlah Blah");
    //            ImGui::TreePop();
    //        }
    //    }
    //    else
    //    {
    //        // Items 3..5 are Tree Leaves
    //        // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
    //        // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
    //        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
    //        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
    //        if (ImGui::IsItemClicked())
    //            node_clicked = i;
    //        if (ImGui::BeginDragDropSource())
    //        {
    //            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
    //            ImGui::Text("This is a drag and drop source");
    //            ImGui::EndDragDropSource();
    //        }
    //    }
    //}
    //if (clickedNode != nullptr)
    //{
    //    // Update selection state
    //    // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
    //    if (ImGui::GetIO().KeyCtrl)
    //        selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
    //    else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
    //        selection_mask = (1 << node_clicked);           // Click to single-select
    //}

    ImGui::End();
}

void UIHierarchy::SetSelectedGameObject(GameObject* newSelected)
{
    selectedGameObject = newSelected;
}

void UIHierarchy::RenderRecursively(GameObject* gameObject)
{
    if (gameObject != nullptr) {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        ImGuiTreeNodeFlags node_flags = base_flags;

        if (gameObject->IsLeaf()) {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            ImGui::TreeNodeEx(gameObject, node_flags, "%s", gameObject->GetName().c_str());

            RenderActionsForGameObject(gameObject);
        }
        else {
            bool node_open = ImGui::TreeNodeEx(gameObject, node_flags, "%s", gameObject->GetName().c_str());

            RenderActionsForGameObject(gameObject);

            if (gameObject != nullptr && node_open)
            {
                for (GameObject* go : gameObject->GetChildren()) {
                    RenderRecursively(go);
                }
                ImGui::TreePop();
            }
        }

    }
}

void UIHierarchy::RenderActionsForGameObject(GameObject* gameObject)
{
    if (ImGui::IsItemClicked()) {
        selectedGameObject = gameObject;
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GAMEOBJECT", (void*)gameObject, sizeof(gameObject));
        char hint[55];
        sprintf_s(hint, 55, "Dropping %s...", gameObject->GetName().c_str());
        ImGui::Text(hint);
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT");
        if (payload)
        {
            LOG(("Dropping " + selectedGameObject->GetName() + " in " + gameObject->GetName()).c_str());
            selectedGameObject->Reparent(gameObject);
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::PushID(gameObject);
    if (ImGui::BeginPopupContextItem("GameObject"))
    {
        if (ImGui::Selectable("Create empty GameObject")) {
            GameObject* emptyGameObject = new GameObject("GameObject(" + std::to_string(emptyGameobjectCounter) + ")", gameObject);
            gameObject->AddGameObject(emptyGameObject);
            emptyGameobjectCounter++;
            LOG(("Created new empty GameObject: " + emptyGameObject->GetName() + " under " + gameObject->GetName()).c_str());
        }

        if (gameObject != App->scene->GetRootNode()) {

            if (!gameObject->IsFirstChildOfParent()) {
                if (ImGui::Selectable("Move up")) {
                    gameObject->MoveUpOnHiearchy();
                }
            }

            if (!gameObject->IsLastChildOfParent()) {
                if (ImGui::Selectable("Move down")) {
                    gameObject->MoveDownOnHierarchy();
                }
            }

            if (ImGui::Selectable("Delete GameObject")) {
                LOG("Removing %s ...", gameObject->GetName().c_str());
                App->scene->RemoveObjectFromScene(gameObject);
                App->scene->ResetQuadtree();
                selectedGameObject = gameObject->GetParent();
                gameObject->RemoveChildFromParent();
                delete gameObject;
                gameObject = nullptr;
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopID();
}
