#pragma once

#include "assimp/mesh.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const aiMesh* mesh);

private:
	int materialIndex;
	int numVertices = 0;
	int numIndices = 0;
};

