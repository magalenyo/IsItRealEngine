#include "FSTexture.h"
#include <IL/il.h>


void FSTexture::ExportTexture()
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);			// To pick a specific DXT compression use
	ilEnable(IL_FILE_OVERWRITE);
	ilSave(IL_DDS, "aaaaaa.dds");
	size = ilSaveL(IL_DDS, nullptr, 0);				// Get the size of the data buffer
	if (size > 0)
	{
		data = new ILubyte[size];					// allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0)		// Save to buffer with the ilSaveIL function
			char* fileBuffer = (char*)data;
	}
}