#include "Model.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "assimp/scene.h"
#include "assimp/cimport.h"		// for aiImportFile
#include "assimp/postprocess.h"	// for aiProcessPreset

#include "MemoryLeakDetector.h"

const std::string Model::PATH_TEXTURES = "./resources/textures/"; 
const std::string Model::PATH_MODELS = "./resources/models/";

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
	/*PATH_TEXTURES.~basic_string();
	PATH_MODELS.~basic_string();*/
}

void Model::Load(const char* file_name)
{
	numVertices = 0;
	numIndices = 0;
	transform.Reset();

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
	for (Mesh* mesh : meshes) {
		delete mesh;
	}

	meshes.clear();

	return true;
}


void Model::Draw()
{
	for (Mesh* mesh : meshes) {
		mesh->Draw(textures);
	}
}

int Model::GetNumMeshes() const
{
	return meshes.size();
}

void Model::GetFirstTextureSize(int &w, int &h)
{
	w = 0; h = 0;
	if (textureSizes.size() > 0) {
		w = textureSizes[0][TEXTURE_SIZES_WIDTH_POSITION];
		h = textureSizes[0][TEXTURE_SIZES_HEIGHT_POSITION];
	}
}

int Model::GetFirstTexture() const
{
	if (textures.size() > 0) {
		return textures[0];
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

Transformation Model::GetTransformation() const
{
	return transform;
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
		meshes.push_back(new Mesh(scene->mMeshes[i]));
		numVertices += meshes[i]->GetNumVertices();
		numIndices += meshes[i]->GetNumIndices();
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

// fastest way to check if a file exists:
// source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool Model::ExistsTexture(const std::string& path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}
