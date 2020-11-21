#pragma once
#include "Module.h"
#include <GL/GL.h>

class ModuleTexture : public Module
{
public:
    ModuleTexture();
    ~ModuleTexture();

    bool Init();
    update_status Update();

    bool LoadTexture(const char* imagePath);
    GLuint GetTextureID() const;

private:
    GLuint textureId;
};

