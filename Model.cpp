#include "Model.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"		// for aiImportFile
#include "assimp/postprocess.h"	// for aiProcessPreset

#include "MemoryLeakDetector.h"

const std::string Model::PATH_TEXTURES			= "./resources/textures/"; 
const std::string Model::PATH_MODELS			= "./resources/models/";
const std::string Model::MODEL_EXTENSION_FBX	= ".fbx";

Model::Model()
{

}

Model::Model(const char* file_name)
{
 	Load(file_name);
}

Model::~Model()
{
	CleanUp();
}

void Model::Load(const char* file_name)
{
	LOG("Loading model: %s...", file_name)
	numVertices = 0;
	numIndices = 0;
	//transform.Reset();

	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene);
		if (textures.size() > 0) {
			LoadMeshes(scene);
			LOG("Model: %s loaded", file_name);
			App->camera->FocusCamera(furthestPosition);
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

void Model::LoadSingleTexture(const std::string &file_name)
{
	LOG("Loading single texture: %s...", file_name.c_str())
	// The texture vector will not be cleared because textures can still be reutilized
	int textureId = App->textures->LoadTexture(file_name.c_str());
	if (textureId != ModuleTexture::TEXTURE_ERROR) {
		textures.push_back(textureId);
		for (ComponentMesh* mesh : meshes) {
			mesh->SetMaterialIndex(textures.size()-1);
		}
		textureSizes.push_back(float2(App->textures->GetTextureWidth(), App->textures->GetTextureHeight()));
		LOG("Texture %s loaded", file_name.c_str())
	}
	else {
		LOG("Error loading texture in an already existing model %s: %s.", file_name.c_str(), aiGetErrorString());
		LOG("Try dropping the texture from a path with no special characters or accents.");
	}
}

bool Model::CleanUp()
{
	for (ComponentMesh* mesh : meshes) {
		delete mesh;
	}

	meshes.clear();
	textureSizes.clear();
	textures.clear();

	/*PATH_TEXTURES.~basic_string();
	PATH_MODELS.~basic_string();
	MODEL_EXTENSION_FBX.~basic_string();*/

	return true;
}


void Model::Draw()
{
	for (ComponentMesh* mesh : meshes) {
		mesh->Draw(textures);
	}
}

int Model::GetNumMeshes() const
{
	return meshes.size();
}

bool Model::CanLoadFBX(const std::string& path)
{
	if (path.length() >= MODEL_EXTENSION_FBX.length()) {
		if (path.compare(path.length() - MODEL_EXTENSION_FBX.length(), MODEL_EXTENSION_FBX.length(), MODEL_EXTENSION_FBX) == 0) {
			return true;
		}
	}
	return false;
}

void Model::GetLastTextureSize(int &w, int &h)
{
	w = 0; h = 0;
	if (textureSizes.size() > 0) {
		w = textureSizes[textureSizes.size() - 1][TEXTURE_SIZES_WIDTH_POSITION];
		h = textureSizes[textureSizes.size() - 1][TEXTURE_SIZES_HEIGHT_POSITION];
	}
}

int Model::GetLastTexture() const
{
	if (textures.size() > 0) {
		return textures[textures.size()-1];
	}
	else return ModuleTexture::TEXTURE_ERROR;
}

int Model::GetNumVertices() const
{
	return numVertices;
}


int Model::GetNumIndices() const
{
	return numIndices;
}

ComponentTransform* Model::GetTransformation() const
{
	//return transform;
	return nullptr;
}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	textures.reserve(scene->mNumMaterials);
	textureSizes.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		aiReturn returnType = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		if (returnType == AI_SUCCESS)
		{
			int textureId = App->textures->LoadTexture(GetProcessedPath(file.data).c_str());
			if (textureId != ModuleTexture::TEXTURE_ERROR) {
				textures.push_back(textureId);
				textureSizes.push_back(float2(App->textures->GetTextureWidth(), App->textures->GetTextureHeight()));
			}
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	meshes.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		meshes.push_back(new ComponentMesh(scene->mMeshes[i], nullptr));
		numVertices += meshes[i]->GetNumVertices();
		numIndices += meshes[i]->GetNumIndices();
		if (meshes[i]->GetFurthestPosition().z > furthestPosition.z) {
			furthestPosition = meshes[i]->GetFurthestPosition();
		}
	}
}

std::string Model::GetProcessedPath(const std::string &path)
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
bool Model::ExistsTexture(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}