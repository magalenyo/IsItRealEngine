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
	const std::string PATH_ASSETS				= "./Assets";
	const std::string PATH_LIBRARY				= "./Library";
	const std::string PATH_LIBRARY_MESHES		= "./Library/Meshes";
	const std::string PATH_LIBRARY_MATERIALS	= "./Library/Materials";
	const std::string PATH_LIBRARY_ANIMATIONS	= "./Library/Animations";

	bool CreateAssetsDirectory();
	bool CreateLibraryDirectory();

};

