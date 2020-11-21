#include "ModuleTexture.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <GL/GL.h>

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	LOG("ModuleTexture - INIT START");
	if (ilGetInteger(IL_VERSION_NUM < IL_VERSION)) {
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

bool ModuleTexture::LoadTexture(const char* imagePath)
{

	// hay que llamar al iluFlipImage antes de coger el data

	
	ILuint newImageId;
	ILboolean success;
	GLuint newTextureId;
	ilGenImages(1, &newImageId); /* Generation of one image name */
	ilBindImage(newImageId); /* Binding of image name */
	success = ilLoadImage(imagePath); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			/* Error occured */
			return false;
		}
		// STORES IN GPU
		iluFlipImage();
		glGenTextures(1, &newTextureId); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, newTextureId); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData()); /* Texture specification */
	}
	else
	{
		/* Error occured */
		return false;
	}
	ilDeleteImages(1, &newTextureId); /* Because we have already copied image data into texture data
	  we can release memory used by image. */

	// THIS NEEDS TO BE RETURNED
	textureId = newTextureId;
	//return newTextureId;
}

GLuint ModuleTexture::GetTextureID() const
{
	return textureId;
}
