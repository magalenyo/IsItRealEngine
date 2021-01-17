#pragma once
#include "Module.h"
#include "Quadtree.h"
#include <string>
#include <vector>
#include "assimp/scene.h"

class GameObject;
class ComponentCamera;
class ComponentMaterial;
class ComponentMesh;

class ModuleScene : public Module
{
public:
    const std::string PATH_TEXTURES         = "./Assets/textures/";
    const std::string PATH_MODELS           = "./Assets/models/";

    bool Init() override;                                       // Initializes root GameObject with children for testing, and also initializes the Frustum Culling camera in the scene.
    update_status Update() override;
    bool CleanUp() override;                                    // Deletes Quadtree and Root node

    void Load(const char* file_name);                           // Loads a GameObject with Root as parent if the FBX from the given file_name can be loaded. Calls to LoadRecursively
    void LoadSingleTexture(const std::string& file_name);       // Loads a texture from the given path and adds it to the currently selected GameObject diffuse texture
    void LoadDroppedFile(std::string path);                     // Loads the drop file from the given path. If it's an image, loads it to the currently selected GameObject. If it's an FBX, loads it with Root as parent.
    Quadtree* GetQuadtree();                                    // Returns this quadtree
    GameObject* GetCamera();                                    // Returns this camera
    GameObject* GetRootNode() const;                            // Returns root node
    void RemoveObjectFromScene(GameObject* gameObject);         // Removes given GameObject from objectsInScene vector
    void ResetQuadtree();                                       // Rebuilds quadtree from objectsInScene
    GameObject* SendRay(LineSegment& picking, float& distance); // Sends the ray passed by parameter to test if it collides with any object on the scene, and if so, returns it

    std::vector<GameObject*> GetObjectsInScene();               // Returns vector objectsInScene

private:
    const std::string MODEL_EXTENSION_FBX   = ".fbx";

    GameObject* root                        = nullptr;
    GameObject* camera                      = nullptr;
    std::vector<GameObject*> objectsInScene;

    Quadtree* quadtree                      = nullptr;

    void TestRay(LineSegment& picking, float& distance, GameObject** picked);                       //Searches if there is and object in the scene that collides with the ray passed by parameter.
    
    
    GameObject* LoadRecursively(const char* file_name, const aiScene* scene, const aiNode* node, GameObject* parent);   // Creates a GameObject and all its hierarchy from the scene of the fbx. This is called for every children.
    ComponentMaterial* LoadMaterial(const char* file_name, const aiMaterial* mMaterial);                                // Loads a material from the aiMaterial. Creates Texture diffuseTexture, specularTexture, normalTexture, emissiveTexture from the material and exports the material as a JSON file



    std::string GetProcessedPath(const char* modelPath, const std::string& textureName);            // Returns the path to the directory where the texture is. ModelPath is the directory from which the fbx has been loaded and textureName is the name from the file.data
                                                                                                    // Returns empty if the file has not been found

    std::string SanitizeTextureName(const std::string& textureName, bool withExtension = true);     // Clears directories of the file_name, for eg: 
                                                                                                    // input: \\textures\\street_environment.tcg
                                                                                                    // returns street_environment.tcg

    bool ExistsTexture(const std::string& path);                                                    // Returns true if the path corresponds to a file or directory


    
};

