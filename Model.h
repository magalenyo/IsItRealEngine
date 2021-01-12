#pragma once

#include "assimp/scene.h"

class Model
{
public:
	static bool CanLoadFBX(const std::string& path);

private:

	static const std::string MODEL_EXTENSION_FBX;

};

