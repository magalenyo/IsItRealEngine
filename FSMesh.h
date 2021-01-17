#pragma once

#include <string>
#include "assimp/mesh.h"

class ComponentMesh;

class FSMesh
{
public:
	static void ExportMesh(const aiMesh* mesh, const std::string& path);
	static ComponentMesh* ImportMaterial(std::string path);
};

