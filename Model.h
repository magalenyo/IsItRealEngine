#pragma once

#include "assimp/scene.h"

class Model
{
public:
	static const std::string PATH_TEXTURES;
	static const std::string PATH_MODELS;

	static bool CanLoadFBX(const std::string& path);

private:

	static const int TEXTURE_SIZES_WIDTH_POSITION = 0;
	static const int TEXTURE_SIZES_HEIGHT_POSITION = 1;
	static const std::string MODEL_EXTENSION_FBX;

};

