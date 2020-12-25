#include "Scene.h"
#include "GL/glew.h"

Scene::Scene()
{
    CreateFBO();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

Scene::~Scene()
{
    DeleteBuffer();
}

void Scene::CreateVAO()
{
    
}

void Scene::CreateFBO()
{
    glGenFramebuffers(1, &fbo);
}

void Scene::CreateTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void Scene::DeleteBuffer()
{
    glDeleteFramebuffers(1, &fbo);
}
