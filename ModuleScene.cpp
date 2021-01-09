#include "ModuleScene.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Texture.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"		// for aiImportFile
#include "assimp/postprocess.h"	// for aiProcessPreset

#include "MemoryLeakDetector.h"

bool ModuleScene::Init()
{
	root = new GameObject("Root GameObject", nullptr);
	//Load("./resources/models/turret cannon multicolored.fbx");
	Load("./resources/scene/Clock/Clock.fbx");
	//Load("E:/Unity/BattleDefense/Assets/Models/Environment/Clock.fbx");
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
		std::vector<ComponentMaterial*> materials = LoadMaterials(file_name, scene->mMaterials, scene->mNumMaterials);

		//if (!materials.empty()) {
		//	LOG("Model: %s loaded", file_name);
		//}
		//else {
		//	LOG("ERROR Loading Model: %s. No texture found.", file_name);
		//}

		GameObject* gameObject = LoadRecursively(scene, scene->mRootNode, root);
		root->AddGameObject(gameObject);
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

GameObject* ModuleScene::GetRootNode() const
{
	return root;
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

// TODO REFACTOR THIS INSTEAD TO RETURN ONE COMPONENTMATERIAL
std::vector<ComponentMaterial*> ModuleScene::LoadMaterials(const char* file_name, aiMaterial** const mMaterials, unsigned int mNumMaterials)
{
	std::vector<ComponentMaterial*> result;
	aiString file;
	for (unsigned i = 0; i < mNumMaterials; ++i)
	{
		
		for (unsigned j = 0; j < mMaterials[i]->mNumProperties ; ++j)
		{
			LOG(mMaterials[i]->mProperties[j]->mKey.C_Str());
		}

		aiString materialName;//The name of the material found in mesh file
		aiReturn ret;//Code which says whether loading something has been successful of not
		ret = mMaterials[i]->Get(AI_MATKEY_NAME, materialName);//Get the material name (pass by reference)e)
		float shininess = 0;
		mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess);

		ComponentMaterial* material = new ComponentMaterial(nullptr);

		aiReturn returnTexture = mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		if (returnTexture == AI_SUCCESS)
		{
			int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
			if (textureId != ModuleTexture::TEXTURE_ERROR) {
				material->AddTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::DIFFUSE));
			}
		}

		returnTexture = mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &file);
		if (returnTexture == AI_SUCCESS)
		{
			int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
			if (textureId != ModuleTexture::TEXTURE_ERROR) {
				material->AddTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::SPECULAR));
			}
		}

		returnTexture = mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &file);
		if (returnTexture == AI_SUCCESS)
		{
			int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
			if (textureId != ModuleTexture::TEXTURE_ERROR) {
				material->AddTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::NORMAL));
			}
		}

		if (material->HasTextures()) {
			result.push_back(material);
		}
		else {
			delete material;
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
		ComponentMesh* mesh = new ComponentMesh(scene->mMeshes[i], nullptr);
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
		ComponentMesh* mesh = new ComponentMesh(scene->mMeshes[node->mMeshes[i]], go);
		go->AddComponent(mesh);
	}

	// Add transformation component
	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scale, rotation, translation);
	go->AddComponent(
		new ComponentTransform(
			float3(translation.x, translation.y, translation.z),
			float3(scale.x, scale.y, scale.z),
			Quat(rotation.x, rotation.y, rotation.z, rotation.w)
			, go)
	);

	// Add Children
	for (int i = 0; i < node->mNumChildren; i++) {
		go->AddGameObject(LoadRecursively(scene, node->mChildren[i], go));
	}

	return go;
}

std::string ModuleScene::GetProcessedPath(const char* modelPath, const std::string& textureName)
{
	std::string currentPath = textureName;

	char sourceDirectory[_MAX_DIR];
	_splitpath_s(modelPath, NULL, 0, sourceDirectory, _MAX_DIR, NULL, 0, NULL, 0);

	if (ExistsTexture(currentPath = std::string(sourceDirectory) + textureName)){
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in fbx path", textureName.c_str());

	if (ExistsTexture(currentPath = textureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in default (GAME) path", textureName.c_str());

	if (ExistsTexture(currentPath = PATH_MODELS + textureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in models path", textureName.c_str());

	if (ExistsTexture(currentPath = PATH_TEXTURES + textureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s not found", textureName.c_str());
	return "";
}

// fastest way to check if a file exists:
// source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool ModuleScene::ExistsTexture(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}