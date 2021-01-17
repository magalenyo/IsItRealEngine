#pragma once

#include<vector>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleCamera;
class ModuleTime;
class ModuleTexture;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleScene;
class ModuleRenderScene;
class SceneImporter;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	bool HasEnded() const;

public:
	ModuleRender* renderer			= nullptr;
	ModuleWindow* window			= nullptr;
	ModuleInput* input				= nullptr;
	ModuleCamera* camera			= nullptr;
	ModuleTime* time				= nullptr;
	ModuleProgram* program			= nullptr;
	ModuleTexture* textures			= nullptr;
	ModuleEditor* editor			= nullptr;
	ModuleDebugDraw* debugDraw		= nullptr;
	ModuleScene* scene				= nullptr;
	ModuleRenderScene* sceneRender	= nullptr;
	SceneImporter* sceneImporter	= nullptr;

private:

	std::vector<Module*> modules;
	bool ended						= false;

};

extern Application* App;
