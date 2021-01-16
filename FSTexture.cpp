#include "FSTexture.h"
#include "SceneImporter.h"
#include <IL/il.h>

#include "MemoryLeakDetector.h"

void FSTexture::ExportTexture(const std::string &name)
{
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);			// To pick a specific DXT compression use
	ilEnable(IL_FILE_OVERWRITE);
	ilSave(IL_DDS, name.c_str());
}