#pragma once

#include "Component.h"
#include <vector>

class GameObject;
class Texture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* owner);
	~ComponentMaterial();

	void AddTexture(Texture* texture);

	bool HasTextures() const;

private:
	std::vector<Texture*> textures;

	Texture* diffuse	= nullptr;
	Texture* specular	= nullptr;
	Texture* emissive	= nullptr;
	Texture* normal		= nullptr;
	float shinines		= 0;

};

