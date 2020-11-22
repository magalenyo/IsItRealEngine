#pragma once

#include "assimp/mesh.h"
#include <vector>

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

private:
	unsigned int materialIndex;
	int numVertices = 0;
	int numIndices = 0;

	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;

	void DestroyBuffer(unsigned int buffer);
};

