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
    const std::string PATH_TEXTURES = "./resources/textures/";
    const std::string PATH_MODELS = "./resources/models/";

    bool Init() override;
    bool CleanUp() override;

    void Load(const char* file_name);
    void LoadSingleTexture(const std::string& file_name);
    void LoadModel(std::string path);

    GameObject* GetRootNode() const;

private:

    const std::string MODEL_EXTENSION_FBX = ".fbx";

    GameObject* root = nullptr;


    // TODO EXTERNALIZE INTO A MODEL LOADER
    //std::vector<ComponentMaterial*> LoadMaterials(const char* file_name, aiMaterial** const mMaterials, unsigned int mNumMaterials);
    ComponentMaterial* LoadMaterials(const char* file_name, aiMaterial* const mMaterial);
    std::vector<ComponentMesh*> LoadMeshes(const aiScene* scene);
    GameObject* LoadRecursively(const char* file_name, const aiScene* scene, const aiNode* node, GameObject* parent);



    std::string GetProcessedPath(const char* modelPath, const std::string& textureName);            // Returns the path to the directory where the texture is. ModelPath is the directory from which the fbx has been loaded and textureName is the name from the file.data
                                                                                                    // Returns empty if the file has not been found

    std::string SanitizeTextureName(const std::string& textureName);                                // Clears directories of the file_name, for eg: 
                                                                                                    // input: \\textures\\street_environment.tcg
                                                                                                    // returns street_environment.tcg

    bool ExistsTexture(const std::string& path);                                                    // Returns true if the path corresponds to a file or directory


    
};

