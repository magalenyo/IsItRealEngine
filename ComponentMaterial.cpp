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

bool ComponentMaterial::HasTextures() const
{
	return !textures.empty();
}
