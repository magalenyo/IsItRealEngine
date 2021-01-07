#pragma once
#include "Module.h"
#include <string>
#include <vector>
#include "assimp/scene.h"

class GameObject;
class ComponentMaterial;
class ComponentMesh;

class ModuleScene : public Module
{
public:
    bool Init() override;
    bool CleanUp() override;

    void Load(const char* file_name);
    void LoadSingleTexture(const std::string& file_name);

    GameObject* GetRootNode() const;

private:
    const std::string PATH_TEXTURES = "./resources/textures/";
    const std::string PATH_MODELS = "./resources/models/";
    const std::string MODEL_EXTENSION_FBX = ".fbx";

    GameObject* root = nullptr;




    // TODO EXTERNALIZE INTO A MODEL LOADER
    std::vector<ComponentMaterial*> LoadMaterials(const aiScene* scene);
    std::vector<ComponentMesh*> LoadMeshes(const aiScene* scene);
    GameObject* LoadRecursively(const aiScene* scene, const aiNode* node, GameObject* parent);


    std::string GetProcessedPath(const std::string& path);
    bool ExistsTexture(const std::string& path);
};

