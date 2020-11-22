#pragma once
#include "Module.h"
#include <GL/GL.h>

class ModuleTexture : public Module
{
public:
    static const int TEXTURE_ERROR = -1;

    ModuleTexture();
    ~ModuleTexture();

    bool Init();
    update_status Update();

    int LoadTexture(const char* imagePath);
};

