#pragma once

#include "assimp/mesh.h"
#include <vector>
#include "Math/float3.h"

class Mesh
{
public:
	Mesh();
	Mesh(const aiMesh* mesh);
	~Mesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void SetMaterialIndex(unsigned int newMaterialIndex);
	void Draw(const std::vector<unsigned>& model_textures);

	bool CleanUp();

	unsigned int GetNumVertices() const;
	unsigned int GetNumIndices() const;
	unsigned int GetTriangles() const;
	unsigned int GetFaces() const;
	float3 GetFurthestPosition();

private:
	static const int BIND_VERTEX_ARRAY_END = 0;

	unsigned int materialIndex = 0;
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;
	unsigned int numFaces = 0;

	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;

	float3 furthestPosition = float3(0, 0, 0);

	void DestroyBuffer(unsigned int buffer);
};

