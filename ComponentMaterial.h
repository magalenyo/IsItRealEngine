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

	void RenderToEditor() override;
	void SetDiffuseColor(float3 _diffuseColor);
	void SetDiffuseTexture(Texture* texture);
	void SetSpecularColor(float3 _specularColor);
	void SetSpecularTexture(Texture* texture);
	void SetNormalTexture(Texture* texture);
	void SetShininess(float _shininess);

	Texture* GetDiffuseTexture() const;

private:
	float3 diffuseColor		= float3(0, 0, 0);
	float3 specularColor	= float3(0, 0, 0);
	Texture* diffuse		= nullptr;
	Texture* specular		= nullptr;
	Texture* emissive		= nullptr;
	Texture* normal			= nullptr;
	float shininess			= 0;

};

