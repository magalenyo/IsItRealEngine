#pragma once

#include "assimp/scene.h"
#include <vector>
#include "Mesh.h"
#include "Math/vec2d.h"

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

	int GetNumVertices() const;
	int GetNumIndices() const;
	int GetNumMeshes() const;
	int GetFirstTexture() const;
	void GetFirstTextureSize(int &w, int &h);


private:

	static const int TEXTURE_SIZES_WIDTH_POSITION = 0;
	static const int TEXTURE_SIZES_HEIGHT_POSITION = 1;

	std::vector<unsigned int> textures; // Texture mesh
	std::vector<float2> textureSizes;	// Texture size
	std::vector<Mesh*> meshes;			// Mesh array
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;
	
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);

	std::string GetProcessedPath(const std::string& path);
	bool ExistsTexture(const std::string& path);
};

