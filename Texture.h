#pragma once

#include <string>
#include "FSJsonIncluders.h"

class Texture
{
public:
	enum TextureType {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		EMISSIVE,
		UNDEFINED
	};

	Texture(unsigned int textureID, float width, float height, TextureType type);
	~Texture();

	void SetWidth(float _width);
	void SetHeight(float _height);
	void SetTextureID(unsigned int _textureID);
	void SetTexturePath(std::string _textureName);

	float GetWidth() const;
	float GetHeight() const;
	unsigned int GetTextureID() const;
	std::string GetTexturePath() const;
	void DeleteTextureID();

	void SerializeExport(Value& value, Document::AllocatorType& allocator);					// This method serializes material's data to be exported as a material file
	static void SerializeEmpty(Value& value, Document::AllocatorType& allocator);			// This can be called statically in case the Texture is null, but returns the same structure than SerializeExport

private:
	unsigned int textureID;
	float width		= 1;
	float height	= 1;
	TextureType type;
	std::string texturePath;
};

