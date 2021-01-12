#include "Model.h"

#include "MemoryLeakDetector.h"

const std::string Model::MODEL_EXTENSION_FBX	= ".fbx";

bool Model::CanLoadFBX(const std::string& path)
{
	if (path.length() >= MODEL_EXTENSION_FBX.length()) {
		if (path.compare(path.length() - MODEL_EXTENSION_FBX.length(), MODEL_EXTENSION_FBX.length(), MODEL_EXTENSION_FBX) == 0) {
			return true;
		}
	}
	return false;
}