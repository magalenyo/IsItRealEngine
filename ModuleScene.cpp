#include "ModuleScene.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Texture.h"
#include "Model.h"
#include "FSConstants.h"

#include "assimp/scene.h"
#include "assimp/cimport.h"		// for aiImportFile
#include "assimp/postprocess.h"	// for aiProcessPreset

#include "FSTexture.h"

#include "MemoryLeakDetector.h"

bool ModuleScene::Init()
{
	root = new GameObject("ROOT", nullptr);
	//Load("./resources/models/turret cannon multicolored.fbx");
	//Load("./resources/scene/Clock/ClockCustom.fbx");
	//Load("./resources/scene/Clock/Clock.fbx");
	//Load("./resources/scene/Firetruck/Firetruck.fbx");
	//Load("./resources/scene/Dollhouse/Dollhouse.fbx");
	//Load("./resources/scene/Zombunny/ZombunnyCustom.fbx");
	//Load("./resources/scene/Zombunny/Zombunny.fbx");

	Load("./resources/models/BakerHouse.fbx");
	//Load("E:/Unity/BattleDefense/Assets/Models/Environment/Clock.fbx");
	//Load("./resources/Street_Environment/Street_environment_V01.FBX");
	return true;
}

bool ModuleScene::CleanUp()
{
	delete root;
	root = nullptr;
	return true;
}

void ModuleScene::Load(const char* file_name)
{
	LOG("Loading model: %s...", file_name);

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		GameObject* gameObject = LoadRecursively(file_name, scene, scene->mRootNode, root);
		root->AddGameObject(gameObject);
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void ModuleScene::LoadModel(std::string path)
{
	LOG("File %s dropped", path.c_str());
	if (ModuleTexture::IsTexture(path)) {
		LOG("File %s is a TEXTURE", path.c_str());
		//loadedModel->LoadSingleTexture(path);
		//TODO WHAT IT DOES?!
	}
	else if (Model::CanLoadFBX(path)) {
		LOG("File %s is a MODEL", path.c_str());
		Load(path.c_str());
	}
	else {
		LOG("%s cannot be loaded", path.c_str());
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
//std::vector<ComponentMaterial*> ModuleScene::LoadMaterials(const char* file_name, aiMaterial** const mMaterials, unsigned int mNumMaterials)
//{
//	std::vector<ComponentMaterial*> result;
//	aiString file;
//	for (unsigned i = 0; i < mNumMaterials; ++i)
//	{
//		
//		for (unsigned j = 0; j < mMaterials[i]->mNumProperties ; ++j)
//		{
//			LOG(mMaterials[i]->mProperties[j]->mKey.C_Str());
//		}
//
//		aiString materialName;//The name of the material found in mesh file
//		aiReturn ret;//Code which says whether loading something has been successful of not
//		ret = mMaterials[i]->Get(AI_MATKEY_NAME, materialName);//Get the material name (pass by reference)e)
//		float shininess = 0;
//		mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess);
//
//		ComponentMaterial* material = new ComponentMaterial(nullptr);
//
//		aiReturn returnTexture = mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file);
//		if (returnTexture == AI_SUCCESS)
//		{
//			int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
//			if (textureId != ModuleTexture::TEXTURE_ERROR) {
//				//material->AddTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::DIFFUSE));
//				material->SetDiffuseTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::DIFFUSE));
//			}
//		}
//
//		returnTexture = mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &file);
//		if (returnTexture == AI_SUCCESS)
//		{
//			int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
//			if (textureId != ModuleTexture::TEXTURE_ERROR) {
//				//material->AddTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::SPECULAR));
//				material->SetSpecularTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::SPECULAR));
//			}
//		}
//
//		returnTexture = mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &file);
//		if (returnTexture == AI_SUCCESS)
//		{
//			int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
//			if (textureId != ModuleTexture::TEXTURE_ERROR) {
//				material->AddTexture(new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::NORMAL));
//			}
//		}
//
//		if (material->HasTextures()) {
//			result.push_back(material);
//		}
//		else {
//			delete material;
//		}
//
//	}
//	return result;
//}

 //TODO REFACTOR THIS INSTEAD TO RETURN ONE COMPONENTMATERIAL
ComponentMaterial* ModuleScene::LoadMaterial(const char* file_name, aiMaterial* const mMaterial)
{
	std::vector<ComponentMaterial*> result;
	aiString file;

	aiString materialName;													//The name of the material found in mesh file
	aiReturn ret = mMaterial->Get(AI_MATKEY_NAME, materialName);			//Get the material name (pass by reference)
	

	ComponentMaterial* material = new ComponentMaterial(nullptr);
	material->SetName(std::string(materialName.C_Str()));

	aiReturn returnTexture = mMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
	if (returnTexture == AI_SUCCESS)
	{
		int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());																// Generate texture to OpenGL
		if (textureId != ModuleTexture::TEXTURE_ERROR) {																										// If loaded correctly
			std::string texturePath = PATH_LIBRARY_MATERIALS + SanitizeTextureName(file.data, false) + FORMAT_DDS;															// Generate name from sanitized name and add the library path
			Texture* texture = new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::DIFFUSE);		// Create Texture Object
			texture->SetTexturePath(texturePath);																												// Set path of generated dds texture
			material->SetDiffuseTexture(texture);																												// Set texture to material
			FSTexture::ExportTexture(texturePath);																												// Export texture to dds
		}
	}

	returnTexture = mMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
	if (returnTexture == AI_SUCCESS)
	{
		int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
		if (textureId != ModuleTexture::TEXTURE_ERROR) {
			std::string texturePath = PATH_LIBRARY_MATERIALS + SanitizeTextureName(file.data, false) + FORMAT_DDS;															// Generate name from sanitized name and add the library path
			Texture* texture = new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::SPECULAR);		// Create Texture Object
			texture->SetTexturePath(texturePath);																												// Set path of generated dds texture
			material->SetSpecularTexture(texture);																												// Set texture to material
			FSTexture::ExportTexture(texturePath);																												// Export texture to dds
		}
	}

	returnTexture = mMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
	if (returnTexture == AI_SUCCESS)
	{
		int textureId = App->textures->LoadTexture(GetProcessedPath(file_name, file.data).c_str());
		if (textureId != ModuleTexture::TEXTURE_ERROR) {
			std::string texturePath = PATH_LIBRARY_MATERIALS + SanitizeTextureName(file.data, false) + FORMAT_DDS;															// Generate name from sanitized name and add the library path
			Texture* texture = new Texture(textureId, App->textures->GetTextureWidth(), App->textures->GetTextureHeight(), Texture::TextureType::NORMAL);		// Create Texture Object
			texture->SetTexturePath(texturePath);																												// Set path of generated dds texture
			material->SetNormalTexture(texture);																												// Set texture to material
			FSTexture::ExportTexture(texturePath);																												// Export texture to dds
		}
	}

	aiColor3D colorDiffuse(0.f, 0.f, 0.f);
	mMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, colorDiffuse);
	material->SetDiffuseColor(float3(colorDiffuse.r, colorDiffuse.g, colorDiffuse.b));

	aiColor3D colorSpecular(0.f, 0.f, 0.f);
	mMaterial->Get(AI_MATKEY_COLOR_SPECULAR, colorSpecular);
	material->SetSpecularColor(float3(colorSpecular.r, colorSpecular.g, colorSpecular.b));

	float shininess = 0;
	mMaterial->Get(AI_MATKEY_SHININESS, shininess);
	material->SetShininess(shininess);

	return material;
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

GameObject* ModuleScene::LoadRecursively(const char* file_name, const aiScene* scene, const aiNode* node, GameObject* parent)
{
	GameObject* go = new GameObject(std::string(node->mName.C_Str()), parent);
	// Add ComponentMesh and ComponentMaterial components
	for (int i = 0; i < node->mNumMeshes; i++) {
		ComponentMesh* mesh = new ComponentMesh(scene->mMeshes[node->mMeshes[i]], go);
		go->AddComponent(mesh);
		ComponentMaterial* material = LoadMaterial(file_name, scene->mMaterials[mesh->GetMaterialIndex()]);
		material->SetParent(go);
		mesh->SetMaterialIndex(i);
		go->AddComponent(material);
	}

	// Add transformation component
	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scale, rotation, translation);

	// TODO SHOULD BE USING ASSIMP'S SCALE BUT SINCE IT'S USING CENTIMETERS SCALE, IT WILL MESS WITH OBJECTS
	// THERE'S N EASY WAY TO SET THE UNIT SCALE FACTOR
	double factor(0.0);
	scene->mMetaData->Get("UnitScaleFactor", factor);
	scene->mMetaData->Get("OriginalUnitScaleFactor", factor);
	

	float scaleFactor = 1;
	if (scale.x == 100) {
		scaleFactor = 0.01;
	}

	ComponentTransform* transform = new ComponentTransform(
		float3(translation.x, translation.y, translation.z),
		float3(scale.x * scaleFactor, scale.y * scaleFactor, scale.z * scaleFactor),
		Quat(rotation.x, rotation.y, rotation.z, rotation.w)
		, go);
	
	transform->CalculateGlobalMatrix();
	go->AddComponent(transform);
	
	
	// Add Children
	for (int i = 0; i < node->mNumChildren; i++) {
		go->AddGameObject(LoadRecursively(file_name, scene, node->mChildren[i], go));
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
	LOG("Trying to clear the path and retry search");

	std::string processedTextureName = SanitizeTextureName(textureName);
	if (ExistsTexture(currentPath = std::string(sourceDirectory) + processedTextureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in fbx path", processedTextureName.c_str());

	if (ExistsTexture(currentPath = processedTextureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in default (GAME) path", processedTextureName.c_str());

	if (ExistsTexture(currentPath = PATH_MODELS + processedTextureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Texture %s NOT found in models path", processedTextureName.c_str());

	if (ExistsTexture(currentPath = PATH_TEXTURES + processedTextureName)) {
		LOG("Found texture in %s", currentPath.c_str());
		return currentPath;
	}
	LOG("Trying to clear the path and retry search");

	LOG("Texture %s not found", processedTextureName.c_str());
	return "";
}

std::string ModuleScene::SanitizeTextureName(const std::string& textureName, bool withExtension)
{
	char textureFilename[_MAX_FNAME];
	char textureExtension[_MAX_EXT];
	_splitpath_s(textureName.c_str(), NULL, 0, NULL, 0, textureFilename, _MAX_FNAME, textureExtension, _MAX_EXT);
	
	return withExtension ? std::string(textureFilename) + std::string(textureExtension) : std::string(textureFilename);
}

// fastest way to check if a file exists:
// source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool ModuleScene::ExistsTexture(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}