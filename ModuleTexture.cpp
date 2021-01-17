#include "ModuleTexture.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <GL/GL.h>


#include "MemoryLeakDetector.h"

const std::string ModuleTexture::TEXTURE_EXTENSION_PNG = ".png";
const std::string ModuleTexture::TEXTURE_EXTENSION_DDS = ".dds";
const std::string ModuleTexture::TEXTURE_EXTENSION_JPG = ".jpg";

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{

}

bool ModuleTexture::Init()
{
	LOG("ModuleTexture - INIT START");
	if (ilGetInteger(IL_VERSION_NUM < IL_VERSION))
	{
		LOG("ModuleTexture - INIT - DevIL version incorrect");
		LOG("ModuleTexture - INIT END");
		return false;
	}
	ilInit();
	LOG("ModuleTexture - INIT END");
	return true;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	TEXTURE_EXTENSION_PNG.~basic_string();
	TEXTURE_EXTENSION_DDS.~basic_string();
	TEXTURE_EXTENSION_JPG.~basic_string();

	return true;
}

int ModuleTexture::LoadTexture(const char* imagePath)
{
	ILuint newImageId;
	ILboolean success;
	GLuint newTextureId;
	ilGenImages(1, &newImageId); /* Generation of one image name */
	ilBindImage(newImageId); /* Binding of image name */
	success = ilLoadImage(imagePath); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			return TEXTURE_ERROR;
		}

		ILinfo imageInfo;
		iluGetImageInfo(&imageInfo);
		// if origin needs to be flipped, flip it
		if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}
		// STORES IN GPU
		glGenTextures(1, &newTextureId); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, newTextureId); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter); /* We will use linear
		  interpolation for minifying filter */
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */	
	}
	else
	{
		ILenum errorType = ilGetError();
		/* Error occured */
		return TEXTURE_ERROR;
	}
	ilDeleteImages(1, &newTextureId); /* Because we have already copied image data into texture data
									  // TODO CHANGING the delete for the newImageId
	  we can release memory used by image. */


	// TODO: reload texture on the openGL buffer only.
	return newTextureId;
}

bool ModuleTexture::IsTexture(const std::string& imagePath)
{
	if (imagePath.length() >= TEXTURE_EXTENSION_LENGTH)
	{
		if (imagePath.compare(imagePath.length() - TEXTURE_EXTENSION_PNG.length(), TEXTURE_EXTENSION_PNG.length(), TEXTURE_EXTENSION_PNG) == 0) 
		{
			return true;
		}
		else if (imagePath.compare(imagePath.length() - TEXTURE_EXTENSION_DDS.length(), TEXTURE_EXTENSION_DDS.length(), TEXTURE_EXTENSION_DDS) == 0) 
		{
			return true;
		}
		else if (imagePath.compare(imagePath.length() - TEXTURE_EXTENSION_JPG.length(), TEXTURE_EXTENSION_JPG.length(), TEXTURE_EXTENSION_JPG) == 0) 
		{
			return true;
		}
	}
	return false;
}
void ModuleTexture::SetDefaultConfig()
{
	SetMagnificationFilter(GL_LINEAR);
	SetMinificationFilter(GL_LINEAR);
}

int ModuleTexture::GetMagnificationFilter()
{
	return magnificationFilter;
}

void ModuleTexture::SetMagnificationFilter(int newFilter)
{
	magnificationFilter = newFilter;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);
}

int ModuleTexture::GetMinificationFilter()
{
	return minificationFilter;
}

void ModuleTexture::SetMinificationFilter(int newFilter)
{
	minificationFilter = newFilter;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
}

int ModuleTexture::GetTextureWidth() const
{
	return ilGetInteger(IL_IMAGE_WIDTH);
}

int ModuleTexture::GetTextureHeight() const
{
	return ilGetInteger(IL_IMAGE_HEIGHT);
}
