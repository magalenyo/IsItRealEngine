#include "GameObject.h"
#include "GUIDGenerator.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

GameObject::GameObject() : uid(GenerateUID()) {}

GameObject::GameObject(const std::string& name, GameObject* parent) : name(name), parent(parent), uid(GenerateUID()) {}

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

void GameObject::Draw() const
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
