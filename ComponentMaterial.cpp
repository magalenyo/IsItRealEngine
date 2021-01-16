#include "ComponentMaterial.h"
#include "imgui.h"
#include "Texture.h"
#include "MemoryLeakDetector.h"
#include "FSMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* owner) : Component(owner, ComponentType::MATERIAL) {}

ComponentMaterial::~ComponentMaterial()
{
	if (diffuse != nullptr) {
		delete diffuse;
		diffuse = nullptr;
	}

	if (specular != nullptr) {
		delete specular;
		specular = nullptr;
	}

	if (normal != nullptr) {
		delete normal;
		normal = nullptr;
	}

	if (emissive != nullptr) {
		delete emissive;
		emissive = nullptr;
	}
	// TODO delete each pointer of texture if needed
}

void ComponentMaterial::RenderToEditor()
{
	ImGui::PushID(GetUID().c_str());
    ImGui::Checkbox("Material component", &enabled);
    if (ImGui::CollapsingHeader("Material"))
    {
		ImGui::TextColored(ImVec4(.38, .54, 1, 1), "Name: %s", name.c_str());
		ImGui::Text("");

		if (diffuse != nullptr) {
			ImGui::Text("Diffuse texture");
			ImGui::Text("Width: %.2f", diffuse->GetWidth()); ImGui::SameLine();
			ImGui::Text("Height: %.2f", diffuse->GetHeight());
			ImGui::Text("");
			ImGui::Image((void*)diffuse->GetTextureID(), ImVec2(50, 50));
			ImGui::Text("");
		}
		
		if (specular != nullptr) {
			ImGui::Text("Specular texture");
			ImGui::Text("Width: %.2f", specular->GetWidth()); ImGui::SameLine();
			ImGui::Text("Height: %.2f", specular->GetHeight());
			ImGui::Text("");
			ImGui::Image((void*)specular->GetTextureID(), ImVec2(50, 50));
			ImGui::Text("");
		}
		
		if (normal != nullptr) {
			ImGui::Text("Normal texture");
			ImGui::Text("Width: %.2f", normal->GetWidth()); ImGui::SameLine();
			ImGui::Text("Height: %.2f", normal->GetHeight());
			ImGui::Text("");
			ImGui::Image((void*)normal->GetTextureID(), ImVec2(50, 50));
			ImGui::Text("");
		}

		if (emissive != nullptr) {
			ImGui::Text("Normal texture");
			ImGui::Text("Width: %.2f", emissive->GetWidth()); ImGui::SameLine();
			ImGui::Text("Height: %.2f", emissive->GetHeight());
			ImGui::Text("");
			ImGui::Image((void*)emissive->GetTextureID(), ImVec2(50, 50));
			ImGui::Text("");
		}

		ImGuiColorEditFlags flags = ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_HEX;

		ImGui::Text("Diffuse color");
		ImGui::Text("");
		ImGui::ColorPicker3("Diffuse color", (float*)& diffuseColor, flags);
		ImGui::Text("");

		ImGui::Text("Specular color");
		ImGui::Text("");
		ImGui::ColorPicker3("Specular color", (float*)& specularColor, flags);
		ImGui::Text("");

		ImGui::DragFloat("Shininess", &shininess, 1, 0, INT_MAX, "%.3f");
		ImGui::Text("");
    }
	ImGui::Separator();
	ImGui::PopID();
}

void ComponentMaterial::SetDiffuseColor(float3 _diffuseColor)
{
	diffuseColor = _diffuseColor;
}

void ComponentMaterial::SetDiffuseTexture(Texture* texture)
{
	diffuse = texture;
}

void ComponentMaterial::SetSpecularColor(float3 _specularColor)
{
	specularColor = _specularColor;
}

void ComponentMaterial::SetSpecularTexture(Texture* texture)
{
	specular = texture;
}

void ComponentMaterial::SetNormalTexture(Texture* texture)
{
	normal = texture;
}

void ComponentMaterial::SetShininess(float _shininess)
{
	shininess = _shininess;
}

void ComponentMaterial::SetName(std::string _name)
{
	name = _name;
}

void ComponentMaterial::SetSerializedName(std::string _serializedName)
{
	serializedName = _serializedName;
}

Texture* ComponentMaterial::GetDiffuseTexture() const
{
	return diffuse;
}

std::string ComponentMaterial::GetName() const
{
	return name;
}

std::string ComponentMaterial::GetSerializedName() const
{
	return serializedName;
}

void ComponentMaterial::Serialize(Value& value, Document::AllocatorType& allocator)
{
	Component::Serialize(value, allocator);
	FSMaterial::ExportMaterial(this);
}

void ComponentMaterial::SerializeExport(Value& value, Document::AllocatorType& allocator)
{
	Component::Serialize(value, allocator);

	value.AddMember("name", StringRef(name.c_str()), allocator);
	value.AddMember("serializedName", StringRef(serializedName.c_str()), allocator);
	value.AddMember("shininess", shininess, allocator);

	Value diffuseColorSerialized(kArrayType);
	diffuseColorSerialized.PushBack(diffuseColor.x, allocator);
	diffuseColorSerialized.PushBack(diffuseColor.y, allocator);
	diffuseColorSerialized.PushBack(diffuseColor.z, allocator);
	value.AddMember("diffuseColor", diffuseColorSerialized, allocator);

	Value specularColorSerialized(kArrayType);
	specularColorSerialized.PushBack(specularColor.x, allocator);
	specularColorSerialized.PushBack(specularColor.y, allocator);
	specularColorSerialized.PushBack(specularColor.z, allocator);
	value.AddMember("specularColor", specularColorSerialized, allocator);

	Value diffuseTextureSerialized(kObjectType);
	if (diffuse != nullptr) {
		diffuse->SerializeExport(diffuseTextureSerialized, allocator);
	}
	else {
		// MIGHT NOT BE NEEDED
		//diffuseTextureSerialized.AddMember("MemberWhoseValueIsNULL", Value());
	}
	value.AddMember("diffuse", diffuseTextureSerialized, allocator);

	Value specularTextureSerialized(kObjectType);
	if (specular != nullptr) {
		specular->SerializeExport(specularTextureSerialized, allocator);
	}
	value.AddMember("specular", specularTextureSerialized, allocator);

	Value normalTextureSerialized(kObjectType);
	if (normal != nullptr) {
		normal->SerializeExport(normalTextureSerialized, allocator);
	}
	value.AddMember("normal", normalTextureSerialized, allocator);

	Value emissiveTextureSerialized(kObjectType);
	if (emissive != nullptr) {
		emissive->SerializeExport(emissiveTextureSerialized, allocator);
	}
	value.AddMember("emissive", emissiveTextureSerialized, allocator);
}

ComponentMaterial* ComponentMaterial::Deserialize(const Value& value)
{
	ComponentMaterial* newMaterial = new ComponentMaterial(nullptr);
	newMaterial->uid = value["uid"].GetString();
	newMaterial->enabled = value["enabled"].GetBool();
	newMaterial->type = static_cast<ComponentType>(value["type"].GetInt());

	newMaterial->name = value["name"].GetString();
	newMaterial->serializedName = value["serializedName"].GetString();
	newMaterial->shininess = value["shininess"].GetFloat();

	const Value& diffuseColorSerialized = value["diffuseColor"];
	auto size = diffuseColorSerialized.Size();
	newMaterial->diffuseColor = float3(diffuseColorSerialized[0].GetFloat(), diffuseColorSerialized[1].GetFloat(), diffuseColorSerialized[2].GetFloat());
	 
	const Value& specularColorSerialized = value["specularColor"];
	newMaterial->specularColor = float3(specularColorSerialized[0].GetFloat(), specularColorSerialized[1].GetFloat(), specularColorSerialized[2].GetFloat());

	const Value& diffuseSerialized = value["diffuse"];
	if (!diffuseSerialized.ObjectEmpty()) {
		float widthSerialized = diffuseSerialized["width"].GetFloat();
		float heightSerialized = diffuseSerialized["height"].GetFloat();
		Texture::TextureType textureTypeSerialized = static_cast<Texture::TextureType>(diffuseSerialized["textureType"].GetInt());
		std::string texturePathSerialized = diffuseSerialized["texturePath"].GetString();
		newMaterial->diffuse = new Texture(0, widthSerialized, heightSerialized, textureTypeSerialized);
		newMaterial->diffuse->SetTexturePath(texturePathSerialized);
	}

	const Value& specularSerialized = value["specular"];
	if (!specularSerialized.ObjectEmpty()) {
		float widthSerialized = specularSerialized["width"].GetFloat();
		float heightSerialized = specularSerialized["height"].GetFloat();
		Texture::TextureType textureTypeSerialized = static_cast<Texture::TextureType>(specularSerialized["textureType"].GetInt());
		std::string texturePathSerialized = specularSerialized["texturePath"].GetString();
		newMaterial->specular = new Texture(0, widthSerialized, heightSerialized, textureTypeSerialized);
		newMaterial->specular->SetTexturePath(texturePathSerialized);
	}

	const Value& normalSerialized = value["normal"];
	if (!normalSerialized.ObjectEmpty()) {
		float widthSerialized = normalSerialized["width"].GetFloat();
		float heightSerialized = normalSerialized["height"].GetFloat();
		Texture::TextureType textureTypeSerialized = static_cast<Texture::TextureType>(normalSerialized["textureType"].GetInt());
		std::string texturePathSerialized = normalSerialized["texturePath"].GetString();
		newMaterial->normal = new Texture(0, widthSerialized, heightSerialized, textureTypeSerialized);
		newMaterial->normal->SetTexturePath(texturePathSerialized);
	}

	const Value& emissiveSerialized = value["emissive"];
	if (!emissiveSerialized.ObjectEmpty()) {
		float widthSerialized = emissiveSerialized["width"].GetFloat();
		float heightSerialized = emissiveSerialized["height"].GetFloat();
		Texture::TextureType textureTypeSerialized = static_cast<Texture::TextureType>(emissiveSerialized["textureType"].GetInt());
		std::string texturePathSerialized = emissiveSerialized["texturePath"].GetString();
		newMaterial->emissive = new Texture(0, widthSerialized, heightSerialized, textureTypeSerialized);
		newMaterial->emissive->SetTexturePath(texturePathSerialized);
	}
	

	return newMaterial;
}
