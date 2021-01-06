#pragma once
#include "Module.h"
#include <string>
#include "assimp/scene.h"

class GameObject;

class ModuleScene : public Module
{
public:
    bool Init() override;
    bool CleanUp() override;

    void Load(const char* file_name);
    void LoadSingleTexture(const std::string& file_name);


private:
    const std::string PATH_TEXTURES = "./resources/textures/";
    const std::string PATH_MODELS = "./resources/models/";
    const std::string MODEL_EXTENSION_FBX = ".fbx";

    GameObject* root = nullptr;




    // TODO EXTERNALIZE INTO A MODEL LOADER
    void LoadMaterials(const aiScene* scene, GameObject* gameObject);
    void LoadMeshes(const aiScene* scene, GameObject* gameObject);

    std::string GetProcessedPath(const std::string& path);
    bool ExistsTexture(const std::string& path);
};

