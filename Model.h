#pragma once

#include "assimp/scene.h"
#include <vector>

class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);

private:
	std::vector<unsigned> materials; // array de texturas
	std::vector<unsigned> mesehes;	// array de mallas
};

