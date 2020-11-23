#pragma once
#include "Module.h"
#include "UIComponent.h"
#include <vector>

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


	//void DrawConsole(bool* p_open, std::string logText);
	bool isReady = false;

	UIConsole* console = nullptr;

private:
	std::vector<UIComponent*> components;
};

