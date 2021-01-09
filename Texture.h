#pragma once
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

	Texture(float width, float height, unsigned int textureID, TextureType type);
	~Texture();

	void SetWidth(float _width);
	void SetHeight(float _height);
	void SetTextureID(unsigned int _textureID);

	float GetWidth();
	float GetHeight();
	unsigned int GetTextureID();

private:
	unsigned int textureID;
	float width = 1;
	float height = 1;
	TextureType type;
};

