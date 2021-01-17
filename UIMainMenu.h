#pragma once
#include "UIComponent.h"
class UIMainMenu : public UIComponent
{
public:
	UIMainMenu();
	~UIMainMenu();

	void Init();
	void Draw();

private:
	bool show_about		= false;
	bool show_license	= false;

	void RenderAbout(bool* p_open);	
};

