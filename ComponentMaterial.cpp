#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* owner) : Component(owner, ComponentType::MATERIAL) {}

ComponentMaterial::~ComponentMaterial()
{
	for (Texture* texture : textures) {
		if (texture != nullptr) {
			delete texture;
		}
	}
	textures.clear();
	// TODO delete each pointer of texture if needed
}

void ComponentMaterial::AddTexture(Texture* texture)
{
	if (texture != nullptr) {
		textures.push_back(texture);
	}
}

void ComponentMaterial::SetDiffuseColor(float3 _diffuseColor)
{
	diffuseColor = _diffuseColor;
}

void ComponentMaterial::SetSpecularColor(float3 _specularColor)
{
	specularColor = _specularColor;
}

void ComponentMaterial::SetShininess(float _shininess)
{
	shininess = _shininess;
}

bool ComponentMaterial::HasTextures() const
{
	return !textures.empty();
}
