#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(unsigned int _textureID, float _width, float _height, GameObject* owner) : textureID(_textureID), width(_width), height(_height), Component(owner, ComponentType::Material) {}

void ComponentMaterial::SetWidth(float _width)
{
	width = _width;
}

void ComponentMaterial::SetHeight(float _height)
{
	height = _height;
}

void ComponentMaterial::SetTextureID(unsigned int _textureID)
{
	textureID = _textureID;
}

float ComponentMaterial::GetWidth()
{
	return width;
}

float ComponentMaterial::GetHeight()
{
	return height;
}

unsigned int ComponentMaterial::GetTextureID()
{
	return textureID;
}
