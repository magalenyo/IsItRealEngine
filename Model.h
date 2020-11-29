#pragma once

#include "assimp/scene.h"
#include <vector>
#include "Mesh.h"

class Model
{
public:
	static const std::string PATH_TEXTURES;
	static const std::string PATH_MODELS;

	Model();
	Model(const char* file_name);
	~Model();

	void Load(const char* file_name);
	bool CleanUp();
	void Draw();

private:

	std::vector<unsigned int> textures; // Texture mesh
	std::vector<Mesh*> meshes;	// Mesh array
	
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);

	std::string GetProcessedPath(const std::string& path);
	bool ExistsTexture(const std::string& path);
};

