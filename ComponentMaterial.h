#pragma once

#include "Component.h"

class GameObject;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(unsigned int _textureID, float _width, float _height, GameObject* owner);

	void SetWidth(float _width);
	void SetHeight(float _height);
	void SetTextureID(unsigned int _textureID);

	float GetWidth();
	float GetHeight();
	unsigned int GetTextureID();

private:
	float width = 1;
	float height = 1;
	unsigned int textureID;
};

