#pragma once
#include "Module.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <string>

class ModuleTexture : public Module
{
public:
    static const int TEXTURE_ERROR = -1;
    
    ModuleTexture();
    ~ModuleTexture();

    bool Init();
    update_status Update();
    bool CleanUp();

    int LoadTexture(const char* imagePath);
    static bool IsTexture(const std::string& imagePath);

    void SetDefaultConfig();
    int GetMagnificationFilter();
    void SetMagnificationFilter(int newFilter);
    int GetMinificationFilter();
    void SetMinificationFilter(int newFilter);

    int GetTextureWidth() const;
    int GetTextureHeight() const;

private:
    static const std::string TEXTURE_EXTENSION_PNG;
    static const std::string TEXTURE_EXTENSION_DDS;
    static const std::string TEXTURE_EXTENSION_JPG;
    static const unsigned short TEXTURE_EXTENSION_LENGTH    = 4;
    int magnificationFilter                                 = GL_LINEAR;
    int minificationFilter                                  = GL_LINEAR;
};

