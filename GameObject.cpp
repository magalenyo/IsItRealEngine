#include "GameObject.h"
#include "GUIDGenerator.h"
#include "imgui.h"
#include "debugdraw.h"
#include "MemoryLeakDetector.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

GameObject::GameObject() : uid(GenerateUID()) {}

GameObject::GameObject(const std::string& name, GameObject* parent) : name(name), parent(parent), uid(GenerateUID()) {}

GameObject::~GameObject()
{
	for (Component* component : components) {
		delete component;
		component = nullptr;
	}
	components.clear();

	for (GameObject* gameObject : children) {
		delete gameObject;
		gameObject = nullptr;
	}
	children.clear();
}

void GameObject::Update()
{

}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::AddGameObject(GameObject* gameObject)
{
	children.push_back(gameObject);
}

void GameObject::RemoveChild(GameObject* child)
{
	
	children.erase(remove(children.begin(), children.end(), child));
}

void GameObject::SetParent(GameObject* gameObject)
{
	parent = gameObject;
}

void GameObject::Reparent(GameObject* newParent)
{
	if (parent != newParent) {
		parent->RemoveChild(this);
		SetParent(newParent);
		newParent->AddGameObject(this);
	}
}

void GameObject::RemoveChildFromParent()
{
	if (parent != nullptr) {
		parent->RemoveChild(this);
		SetParent(nullptr);
	}
}

void GameObject::MoveUpOnHiearchy()
{
	for (int i = 0; i < parent->children.size(); ++i) {
		if (parent->children[i] == this) {
			GameObject* aux = parent->children[i];
			parent->children[i] = parent->children[i - 1];
			parent->children[i - 1] = aux;
			break;
		}
	}
}

void GameObject::MoveDownOnHierarchy()
{
	for (int i = 0; i < parent->children.size(); ++i) {
		if (parent->children[i] == this) {
			GameObject* aux = parent->children[i];
			parent->children[i] = parent->children[i + 1];
			parent->children[i + 1] = aux;
			break;
		}
	}
}

void GameObject::Draw() const
{
	if (parent != nullptr)
	{
		if (enabled && parent->IsEnabled())
		{
			ComponentTransform* transform = GetComponent<ComponentTransform>();
			std::vector<ComponentMesh*> meshes = GetComponents<ComponentMesh>();
			std::vector<ComponentMaterial*> materials = GetComponents<ComponentMaterial>();

			for (ComponentMesh* mesh : meshes)
			{
				mesh->Draw(materials, transform->GetGlobalModelMatrix());
			}

			ComponentCamera* camera = GetComponent<ComponentCamera>();
			if (camera != nullptr)
			{
				camera->SetNewPosition(transform->GetPosition());
				dd::frustum((camera->GetFrustum().ProjectionMatrix() * camera->GetFrustum().ViewMatrix()).Inverted(), dd::colors::White);

			}
		}
	}
}

bool GameObject::HasComponents() const
{
	return !components.empty();
}

bool GameObject::IsLeaf() const
{
	return children.empty();
}

bool GameObject::IsFirstChildOfParent() const
{
	if (parent->children[0] == this) {
		return true;
	}
	return false;
}

bool GameObject::IsLastChildOfParent() const
{
	if (parent->children[parent->children.size() - 1] == this) {
		return true;
	}
	return false;
}

std::string GameObject::GetName() const
{
	return name;
}

std::vector<GameObject*> GameObject::GetChildren() const
{
	return children;
}

void GameObject::RenderToEditor()
{
	ImGui::Checkbox("Enabled", &enabled); ImGui::SameLine();

	std::string editorTitle = name;
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(0, 255, 255)));
	ImGui::TextWrapped(editorTitle.c_str());
	ImGui::PopStyleColor(1);


	std::string editorUID = " [UID: " + uid + "]";
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(245, 66, 66)));
	ImGui::TextWrapped(editorUID.c_str());
	ImGui::PopStyleColor(1);


	ImGui::Separator();
	if (enabled) {
		for (Component* component : components) {
			component->RenderToEditor();
		}
	}

}

GameObject* GameObject::GetParent()
{
	return parent;
}
