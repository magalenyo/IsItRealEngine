#pragma once

#include "Component.h"
#include <vector>
#include "Math/float3.h"

class GameObject;
class Texture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* owner);
	~ComponentMaterial();

	void AddTexture(Texture* texture);
	void SetDiffuseColor(float3 _diffuseColor);
	void SetDiffuseTexture(Texture* texture);
	void SetSpecularColor(float3 _specularColor);
	void SetSpecularTexture(Texture* texture);
	void SetShininess(float _shininess);

	bool HasTextures() const;
	Texture* GetDiffuseTexture() const;

private:
	std::vector<Texture*> textures;

	float3 diffuseColor		= float3(0, 0, 0);
	float3 specularColor	= float3(0, 0, 0);
	Texture* diffuse		= nullptr;
	Texture* specular		= nullptr;
	Texture* emissive		= nullptr;
	Texture* normal			= nullptr;
	float shininess			= 0;

};

