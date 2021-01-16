#pragma once

#include <string>

class ComponentMaterial;

class FSMaterial
{
public:
	static void ExportMaterial(ComponentMaterial* material);
	static ComponentMaterial* ImportMaterial(std::string path);
};

