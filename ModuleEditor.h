#pragma once
#include "Module.h"
#include "UIComponent.h"
#include <vector>
#include "Math/float2.h"

class UIMainMenu;
class UIConsole;
class UIConfiguration;
class UIProperties;
class UISceneView;
class UIHierarchy;
class GameObject;

class ModuleEditor : public Module
{
public:

	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	bool CleanUp() override;
	update_status Update() override;
	update_status PreUpdate() override;
	update_status PostUpdate() override;
	void ExitApplication(); // Sets the value of updateStatus to UPDATE_STOP

	bool& GetShowConsole();
	bool& GetShowConfiguration();
	bool& GetShowProperties();
	bool& GetShowSceneView();
	bool& GetShowHierarchyView();
	GameObject* GetSelectedGameObject();
	void SetSelectedGameObject(GameObject* newSelected);
	float2 GetSceneviewPosition() const;

public:
	bool isReady = false;
	UIMainMenu* mainMenu = nullptr;
	UIConsole* console = nullptr;
	UIConfiguration* configuration = nullptr;
	UIProperties* properties = nullptr;
	UISceneView* scene = nullptr;
	UIHierarchy* hierarchy = nullptr;

private:
	std::vector<UIComponent*> components;				// The list of UIComponents of the Editor
	update_status updateStatus = UPDATE_CONTINUE;		// The return type for the Update function

	bool showMainMenu = true;
	bool showConsole = true;
	bool showConfiguration = true;
	bool showProperties = true;
	bool showScene = true;
	bool showHierarchy = true;
};

