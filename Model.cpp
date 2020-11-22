#include "Model.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"		// for aiImportFile
#include "assimp/postprocess.h"	// for aiProcessPreset

const std::string PATH_TEXTURES = "./resources/textures/"; 
const std::string PATH_MODELS = "./resources/models/";

Model::Model()
{

}

Model::Model(const char* file_name)
{
	Load(file_name);
}

Model::~Model()
{
}

void Model::Load(const char* file_name)
{
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene);
		LoadMeshes(scene);
	}
	else
	{ 
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

bool Model::CleanUp()
{
	//TODO

	for (Mesh* mesh : meshes) {
		mesh->CleanUp();
	}
	return true;
}

void Model::Draw()
{
	for (Mesh* mesh : meshes) {
		mesh->Draw(textures);
	}
}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	textures.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			int textureId = App->textures->LoadTexture(GetProcessedPath(file.data).c_str());
			if (textureId != ModuleTexture::TEXTURE_ERROR) {
				textures.push_back(textureId);
			}
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	meshes.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		meshes.push_back(new Mesh(scene->mMeshes[i]));
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
	LOG("Texture %s not found", path);
	return "";
}

bool Model::ExistsTexture(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}
