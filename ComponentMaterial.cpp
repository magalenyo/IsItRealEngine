#include "ComponentMaterial.h"
#include "imgui.h"
#include "Texture.h"

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
	// TODO delete each pointer of texture if needed
}

void ComponentMaterial::RenderToEditor()
{
    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
	ImGui::PushID(GetUID().c_str());
    ImGui::Checkbox("Material component", &enabled);
    if (ImGui::CollapsingHeader("Material"))
    {
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

		//ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayHSV;
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
	ImGui::PopID();
	ImGui::Separator();
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

Texture* ComponentMaterial::GetDiffuseTexture() const
{
	return diffuse;
}
