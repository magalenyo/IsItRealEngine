#pragma once

#include "assimp/scene.h"
#include <vector>
#include "Mesh.h"
#include "Math/float2.h"
#include "Math/float3.h"
#include "Transformation.h"

class Model
{
public:
	static const std::string PATH_TEXTURES;
	static const std::string PATH_MODELS;

	Model();
	Model(const char* file_name);
	~Model();

	void Load(const char* file_name);
	void LoadSingleTexture(const std::string &file_name);
	bool CleanUp();
	void Draw();

	static bool CanLoadFBX(const std::string& path);

	int GetNumVertices() const;
	int GetNumIndices() const;
	int GetNumMeshes() const;
	int GetLastTexture() const;
	void GetLastTextureSize(int &w, int &h);
	Transformation GetTransformation() const;

private:

	static const int TEXTURE_SIZES_WIDTH_POSITION = 0;
	static const int TEXTURE_SIZES_HEIGHT_POSITION = 1;
	static const std::string MODEL_EXTENSION_FBX;

	std::vector<unsigned int> textures; // Texture mesh
	std::vector<float2> textureSizes;	// Texture size
	std::vector<Mesh*> meshes;			// Mesh array
	unsigned int numVertices = 0;		// Number of vertices total. Sum of the vertices of each mesh.
	unsigned int numIndices = 0;		// Number of indices total. Sum of the indices of each mesh.
	Transformation transform;			// Stores Position, Scale and Rotation of the object. Position initialized to (0,0,0)
	float3 furthestPosition = float3(0, 0, 0);	// Store the furthest vertice of the model.
	
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);

	std::string GetProcessedPath(const std::string& path);
	bool ExistsTexture(const std::string& path);


};

