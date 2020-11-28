#pragma once
#include "Module.h"
#include "UIComponent.h"

#include <vector>

class UIMainMenu;
class UIConsole;

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


	bool isReady = false;
	UIMainMenu* mainMenu = nullptr;
	UIConsole* console = nullptr;

private:
	std::vector<UIComponent*> components;				// The list of UIComponents of the Editor
	update_status updateStatus = UPDATE_CONTINUE;		// The return type for the Update function
};

