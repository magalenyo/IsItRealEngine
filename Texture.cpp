#include "Texture.h"
#include "GL/glew.h"

Texture::Texture(float width, float height, unsigned int textureID, TextureType type) : width(width), height(height), textureID(textureID), type(type) { }

Texture::~Texture()
{
	// TODO MAYBE CALL DELETE BUFFER OR SOMETHING?
	// THIS NEEDS TESTING
	glDeleteTextures(1, &textureID);
}

void Texture::SetWidth(float _width)
{
	width = _width;
}

void Texture::SetHeight(float _height)
{
	height = _height;
}

void Texture::SetTextureID(unsigned int _textureID)
{
	textureID = _textureID;
}

float Texture::GetWidth() const
{
	return width;
}

float Texture::GetHeight() const
{
	return height;
}

unsigned int Texture::GetTextureID() const
{
	return textureID;
}