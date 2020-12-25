#pragma once
#include "Module.h"
#include "UIComponent.h"

#include <vector>

class UIMainMenu;
class UIConsole;
class UIConfiguration;
class UIProperties;
class UISceneView;

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool CleanUp();
	update_status Update();
	update_status PreUpdate();
	void ExitApplication(); // Sets the value of updateStatus to UPDATE_STOP

	bool& GetShowConsole();
	bool& GetShowConfiguration();
	bool& GetShowProperties();

public:
	bool isReady = false;
	UIMainMenu* mainMenu = nullptr;
	UIConsole* console = nullptr;
	UIConfiguration* configuration = nullptr;
	UIProperties* properties = nullptr;
	UISceneView* scene = nullptr;

private:
	std::vector<UIComponent*> components;				// The list of UIComponents of the Editor
	update_status updateStatus = UPDATE_CONTINUE;		// The return type for the Update function

	bool show_mainMenu = true;
	bool show_console = true;
	bool show_configuration = true;
	bool show_properties = true;
	bool show_scene = true;
};

