#include "SceneImporter.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include <iostream>
#include <fstream>
#include "FSJsonIncluders.h"
#include "FSConstants.h"


bool SceneImporter::Init()
{
	if (!CreateAssetsDirectory()) {					// If there's been an error creating the Assets Directory
		LOG("[ERROR] %s directory NOT SET UP", PATH_ASSETS.c_str());
		return false;
	}
	LOG("%s directory properly set up", PATH_ASSETS.c_str());
	if (!CreateLibraryDirectory()) {				// If there's been an error creating the Library Directory
		LOG("[ERROR] %s directory NOT SET UP", PATH_LIBRARY.c_str());
		return false;
	}
	LOG("%s directory properly set up", PATH_LIBRARY.c_str());

	// Import scene

	return true;
}

bool SceneImporter::CleanUp()
{
	//ExportScene(App->scene->GetRootNode());
	return true;
}

// fastest way to check if a file exists:
// source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool SceneImporter::ExistsInSystem(const std::string& path)
{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
}

void SceneImporter::ExportScene(GameObject* scene)
{
	if (scene != nullptr) {
		const char json[]  = "{\"root\":}";

		Document document;
		document.SetObject();
		document.Parse(json);
		Document::AllocatorType& allocator = document.GetAllocator();

		Value rootNodeValue(kObjectType);
		scene->Serialize(rootNodeValue, allocator);
		document.AddMember("root", rootNodeValue, allocator);

		StringBuffer buffer;
		//PrettyWriter<StringBuffer> writer(buffer);	// -> beautified version
		Writer<StringBuffer> writer(buffer);			// -> minified version
		document.Accept(writer);

		std::ofstream myfile;
		myfile.open("root.scene");
		myfile << buffer.GetString();
		myfile.close();
	}
}

GameObject* SceneImporter::ImportScene(const char* json)
{
	return nullptr;
}

bool SceneImporter::CreateAssetsDirectory()
{
	if (!ExistsInSystem(PATH_ASSETS)) {
		if (CreateDirectory(PATH_ASSETS.c_str(), NULL)) {
			//SetFileAttributes(PATH_ASSETS.c_str(), FILE_ATTRIBUTE_HIDDEN);
			LOG("Assets directory created successfully");
		}
		else {
			LOG("[ERROR] Assets directory NOT CREATED");
			return false;
		}
	}
	return true;
}

bool SceneImporter::CreateLibraryDirectory()
{
	if (!ExistsInSystem(PATH_LIBRARY)) {
		if (CreateDirectory(PATH_LIBRARY.c_str(), NULL)) {
			SetFileAttributes(PATH_LIBRARY.c_str(), FILE_ATTRIBUTE_HIDDEN);
			LOG("Library directory created successfully");
		}
		else {
			LOG("[ERROR] Library directory NOT CREATED");
			return false;
		}
	}

	if (!ExistsInSystem(PATH_LIBRARY_MESHES)) {
		if (CreateDirectory(PATH_LIBRARY_MESHES.c_str(), NULL)) {
			SetFileAttributes(PATH_LIBRARY_MESHES.c_str(), FILE_ATTRIBUTE_HIDDEN);
			LOG("Library Meshes directory created successfully");
		}
		else {
			LOG("[ERROR] Library Meshes directory NOT CREATED");
			return false;
		}
	}

	if (!ExistsInSystem(PATH_LIBRARY_MATERIALS)) {
		if (CreateDirectory(PATH_LIBRARY_MATERIALS.c_str(), NULL)) {
			SetFileAttributes(PATH_LIBRARY_MATERIALS.c_str(), FILE_ATTRIBUTE_HIDDEN);
			LOG("Library Materials directory created successfully");
		}
		else {
			LOG("[ERROR] Library Materials directory NOT CREATED");
			return false;
		}
	}

	if (!ExistsInSystem(PATH_LIBRARY_ANIMATIONS)) {
		if (CreateDirectory(PATH_LIBRARY_ANIMATIONS.c_str(), NULL)) {
			SetFileAttributes(PATH_LIBRARY_ANIMATIONS.c_str(), FILE_ATTRIBUTE_HIDDEN);
			LOG("Library Animations directory created successfully");
		}
		else {
			LOG("[ERROR] Library Animations directory NOT CREATED");
			return false;
		}
	}

	return true;
}
