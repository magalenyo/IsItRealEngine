#include "Model.h"

#include "MemoryLeakDetector.h"

bool Model::CanLoadFBX(const std::string& path)
{
	std::string MODEL_EXTENSION_FBX = ".fbx";
	std::string MODEL_EXTENSION_FBX_CAPS = ".FBX";

	char extension[_MAX_EXT];
	_splitpath_s(path.c_str(), NULL, 0, NULL, 0, NULL, 0, extension, _MAX_EXT);

	if (strcmp(extension, MODEL_EXTENSION_FBX.c_str()) == 0 || strcmp(extension, MODEL_EXTENSION_FBX_CAPS.c_str()) == 0) {
		return true;
	}
	return false;
}