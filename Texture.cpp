#include "Texture.h"
#include "GL/glew.h"

#include "MemoryLeakDetector.h"


Texture::Texture(unsigned int textureID, float width, float height, TextureType type) : textureID(textureID), width(width), height(height), type(type) { }

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

void Texture::SetTexturePath(std::string _texturePath)
{
	texturePath = _texturePath;
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

std::string Texture::GetTexturePath() const
{
	return texturePath;
}

void Texture::SerializeExport(Value& value, Document::AllocatorType& allocator)
{
	value.AddMember("width", width, allocator);
	value.AddMember("height", height, allocator);
	value.AddMember("textureType", type, allocator);
	value.AddMember("texturePath", StringRef(texturePath.c_str()), allocator);
}

void Texture::SerializeEmpty(Value& value, Document::AllocatorType& allocator)
{
	value.AddMember("width", "", allocator);
	value.AddMember("height", "", allocator);
	value.AddMember("textureType", "", allocator);
	value.AddMember("texturePath", "", allocator);
}
