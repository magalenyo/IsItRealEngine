#include "ModuleScene.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"		// for aiImportFile
#include "assimp/postprocess.h"	// for aiProcessPreset

#include "MemoryLeakDetector.h"

bool ModuleScene::Init()
{
	root = new GameObject("Root GameObject", nullptr);
	Load("./resources/models/turret cannon.fbx");
	return true;
}

bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::Load(const char* file_name)
{
	LOG("Loading model: %s...", file_name);

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		//GameObject* gameObject = new GameObject(std::string(scene->mRootNode->mName.C_Str()), root);
		std::vector<ComponentMaterial*> materials = LoadMaterials(scene);
		materials.push_back(0);
		if (!materials.empty()) {
			//std::vector<ComponentMesh*> meshes = LoadMeshes(scene);
			LOG("Model: %s loaded", file_name);

			GameObject* gameObject = LoadRecursively(scene, scene->mRootNode, root);
			root->AddGameObject(gameObject);

		}
		else {
			LOG("ERROR Loading Model: %s. No texture found.", file_name);
		}
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

//void ModuleScene::LoadSingleTexture(const std::string& file_name)
//{
//	LOG("Loading single texture: %s...", file_name.c_str())
//	// The texture vector will not be cleared because textures can still be reutilized
//	int textureId = App->textures->LoadTexture(file_name.c_str());
//	if (textureId != ModuleTexture::TEXTURE_ERROR) {
//		textures.push_back(textureId);
//		for (ComponentMesh* mesh : meshes) {
//			mesh->SetMaterialIndex(textures.size() - 1);
//		}
//		textureSizes.push_back(float2(App->textures->GetTextureWidth(), App->textures->GetTextureHeight()));
//		LOG("Texture %s loaded", file_name.c_str())
//	}
//	else {
//		LOG("Error loading texture in an already existing model %s: %s.", file_name.c_str(), aiGetErrorString());
//		LOG("Try dropping the texture from a path with no special characters or accents.");
//	}
//}

std::vector<ComponentMaterial*> ModuleScene::LoadMaterials(const aiScene* scene)
{
	std::vector<ComponentMaterial*> result;
	aiString file;
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		aiReturn returnType = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		if (returnType == AI_SUCCESS)
		{
			int textureId = App->textures->LoadTexture(GetProcessedPath(file.data).c_str());
			if (textureId != ModuleTexture::TEXTURE_ERROR) {
				result.push_back(new ComponentMaterial(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight()));
			}
		}
	}
	return result;
}

std::vector<ComponentMesh*> ModuleScene::LoadMeshes(const aiScene* scene)
{
	std::vector<ComponentMesh*> result;
	float3 furthestPosition = float3(0, 0, 0);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		ComponentMesh* mesh = new ComponentMesh(scene->mMeshes[i]);
		result.push_back(mesh);
		if (mesh->GetFurthestPosition().z > furthestPosition.z) {
			furthestPosition = mesh->GetFurthestPosition();
		}
	}
	App->camera->FocusCamera(furthestPosition);
	return result;
}

GameObject* ModuleScene::LoadRecursively(const aiScene* scene, const aiNode* node, GameObject* parent)
{
	GameObject* go = new GameObject(std::string(node->mName.C_Str()), parent);
	// Add node components
	for (int i = 0; i < node->mNumMeshes; i++) {
		ComponentMesh* mesh = new ComponentMesh(scene->mMeshes[node->mMeshes[i]]);
		go->AddComponent(mesh);
	}

	// Add Children
	for (int i = 0; i < node->mNumChildren; i++) {
		go->AddGameObject(LoadRecursively(scene, node->mChildren[i], go));
	}

	return go;
}

std::string ModuleScene::GetProcessedPath(const std::string& path)
{
	std::string currentPath = path;
	if (ExistsTexture(currentPath)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in default path", path.c_str());

	if (ExistsTexture(currentPath = PATH_MODELS + path)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in models path", path.c_str());

	if (ExistsTexture(currentPath = PATH_TEXTURES + path)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s not found", path.c_str());
	return "";
}

// fastest way to check if a file exists:
// source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool ModuleScene::ExistsTexture(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}
