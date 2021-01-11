#include "GameObject.h"
#include "GUIDGenerator.h"
#include "imgui.h"
#include "MemoryLeakDetector.h"
#include "Component.h"

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

bool GameObject::HasComponents() const
{
	return !components.empty();
}

bool GameObject::IsLeaf() const
{
	return children.empty();
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
