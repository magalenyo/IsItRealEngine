#include "GameObject.h"
#include "Application.h"
#include "ModuleRender.h"
#include "GUIDGenerator.h"
#include "imgui.h"
#include "debugdraw.h"
#include "MemoryLeakDetector.h"
#include "Component.h"
#include "ComponentCamera.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Math/float3x3.h"

GameObject::GameObject() : uid(GenerateUID()) {}

GameObject::GameObject(const std::string& name, GameObject* parent) : name(name), parent(parent), uid(GenerateUID()) {}

GameObject::~GameObject()
{
	for (Component* component : components) 
	{
		delete component;
		component = nullptr;
	}
	components.clear();

	for (GameObject* gameObject : children) 
	{
		delete gameObject;
		gameObject = nullptr;
	}
	children.clear();
}

void GameObject::Update()
{
	vec minPoint = vec(FLOAT_INF, FLOAT_INF, FLOAT_INF);
	vec maxPoint = vec(-FLOAT_INF, -FLOAT_INF, -FLOAT_INF);
	ComponentTransform* transform = GetComponent<ComponentTransform>();
	float4x4 modelMatrix = transform->GetGlobalModelMatrix();

	if (!children.empty())
	{
		for (GameObject* child : children)
		{
			child->calculateAABBbounds(minPoint, maxPoint);
			modelMatrix = child->GetComponent<ComponentTransform>()->GetGlobalModelMatrix();
		}

		localaabb = AABB(minPoint, maxPoint);
		//localaabb.SetFromCenterAndSize(transform->GetPosition(), transform->GetScale());
	}
		
	obb = localaabb.Transform(modelMatrix);
	aabb = obb.MinimalEnclosingAABB();

	
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
	if (parent != newParent) 
	{
		parent->RemoveChild(this);
		SetParent(newParent);
		newParent->AddGameObject(this);
	}
}

void GameObject::RemoveChildFromParent()
{
	if (parent != nullptr) 
	{
		parent->RemoveChild(this);
		SetParent(nullptr);
	}
}

void GameObject::MoveUpOnHiearchy()
{
	for (int i = 0; i < parent->children.size(); ++i) 
	{
		if (parent->children[i] == this) 
		{
			GameObject* aux = parent->children[i];
			parent->children[i] = parent->children[i - 1];
			parent->children[i - 1] = aux;
			break;
		}
	}
}

void GameObject::MoveDownOnHierarchy()
{
	for (int i = 0; i < parent->children.size(); ++i) 
	{
		if (parent->children[i] == this) 
		{
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
				float3x3 rotationMatrix = float3x3::FromQuat(transform->GetRotation());
				Frustum frustum = camera->GetFrustum();
				frustum.SetFront(rotationMatrix * float3::unitZ);
				frustum.SetUp(rotationMatrix * float3::unitY);
				camera->SetFrustum(frustum);
				dd::frustum((camera->GetFrustum().ProjectionMatrix() * camera->GetFrustum().ViewMatrix()).Inverted(), dd::colors::Red);
			}
		}
		if (drawAABB)
		{
			dd::aabb(aabb.minPoint, aabb.maxPoint, dd::colors::Green);
		}
		if (drawOBB)
		{
			float3 points[8];
			obb.GetCornerPoints(points);
			dd::box(points, dd::colors::Orange);
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
	if (parent->children[0] == this) 
	{
		return true;
	}
	return false;
}

bool GameObject::IsLastChildOfParent() const
{
	if (parent->children[parent->children.size() - 1] == this) 
	{
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
	if (parent != nullptr) //To not show properties on ROOT (Scene) node
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

		if(GetComponent<ComponentCamera>() == nullptr)
		{
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Draw Boxes"))
			{
				ImGui::Checkbox("Draw AABBs", &drawAABB);

				ImGui::Checkbox("Draw OBBs", &drawOBB);
			}
		}

		ImGui::Separator();
		if (enabled) 
		{
			for (Component* component : components) {
				component->RenderToEditor();
			}
		}
	}

}

GameObject* GameObject::GetParent()
{
	return parent;
}

void GameObject::Serialize(Value &value, Document::AllocatorType& allocator)
{
	value.AddMember("uid", StringRef(uid.c_str()), allocator);
	value.AddMember("name", StringRef(name.c_str()), allocator);
	value.AddMember("enabled", enabled, allocator);
	Value serializedComponentList(kArrayType);
	for (Component* component : components) 
	{
		Value serializedComponent(kObjectType);
		component->Serialize(serializedComponent, allocator);
		serializedComponentList.PushBack(serializedComponent, allocator);
	}
	value.AddMember("components", serializedComponentList, allocator);

	Value serializedGameObjectList(kArrayType);
	for (GameObject* gameObject : children) 
	{
		Value serializedGameObject(kObjectType);
		gameObject->Serialize(serializedGameObject, allocator);
		serializedGameObjectList.PushBack(serializedGameObject, allocator);
	}
	value.AddMember("children", serializedGameObjectList, allocator);
}

GameObject* GameObject::Deserialize(Value& value, GameObject* parent)
{
	GameObject* result = new GameObject();

	result->uid = value["uid"].GetString();
	result->parent = parent;
	result->name = value["name"].GetString();
	result->enabled = value["enabled"].GetBool();

	std::vector<Component*> newComponents;
	Value newComponentsSerialized = value["components"].GetArray();
	if (newComponentsSerialized.Size() > 0)
	{

	}
	result->components = newComponents;

	std::vector<GameObject*> newChildren;
	Value newChildrenSerialized = value["children"].GetArray();
	for (int i = 0; i < newChildrenSerialized.Size(); ++i) 
	{
		GameObject* newChild = GameObject::Deserialize(newChildrenSerialized[i], result);
		newChildren.push_back(newChild);
	}
	result->children = newChildren;

	return result;
}


void GameObject::SetAABB(AABB localAABB)
{
	localaabb = localAABB;
}

void GameObject::calculateAABBbounds(vec& minPoint, vec& maxPoint)
{
	if(localaabb.minPoint.x < minPoint.x)
	{
		minPoint.x = localaabb.minPoint.x;
	}
	if (localaabb.minPoint.y < minPoint.y)
	{
		minPoint.y = localaabb.minPoint.y;
	}
	if (localaabb.minPoint.z < minPoint.z)
	{
		minPoint.z = localaabb.minPoint.z;
	}
	if (localaabb.maxPoint.x > maxPoint.x)
	{
		maxPoint.x = localaabb.maxPoint.x;
	}
	if (localaabb.maxPoint.y > maxPoint.y)
	{
		maxPoint.y = localaabb.maxPoint.y;
	}
	if (localaabb.maxPoint.z > maxPoint.z)
	{
		maxPoint.z = localaabb.maxPoint.z;
	}
}