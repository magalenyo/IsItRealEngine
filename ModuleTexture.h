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

    void SetDefaultConfig();
    int GetMagnificationFilter();
    void SetMagnificationFilter(int newFilter);
    int GetMinificationFilter();
    void SetMinificationFilter(int newFilter);

    int GetTextureWidth() const;
    int GetTextureHeight() const;

private: 
    int magnificationFilter = GL_LINEAR;
    int minificationFilter = GL_LINEAR;
};

