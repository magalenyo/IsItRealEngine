#pragma once

#include "Module.h"
#include <string>
#include "GameObject.h"

class SceneImporter : public Module
{
public:

	bool Init() override;
	bool CleanUp() override;

	bool ExistsInSystem(const std::string& path);
	void ExportScene(GameObject* scene);
	GameObject* ImportScene(const char* json);

private:
	bool CreateAssetsDirectory();
	bool CreateLibraryDirectory();

};

