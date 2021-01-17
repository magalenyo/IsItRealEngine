#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	enum WindowType 
	{
		W_FULLSCREEN = 0,
		W_FULLSCREEN_DESKTOP = 1,
		W_WINDOW_BORDERLESS = 2,
		W_WINDOW_RESIZABLE = 3
	};

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	update_status Update();

	int GetWindowWidth() const;
	int GetWindowHeight() const;
	void GetWindowSize(int &w, int &h) const;

	int GetWindowMaxWidth() const;
	int GetWindowMaxHeight() const;
	void GetWindowMaxSize(int& w, int& h) const;

	void SetWindowSize(int w, int h);

	/*void TurnVSYNC(bool param);*/

	void SetWindowConfiguration();

	int GetWindowType();
	void SetWindowType(int type);

	SDL_Window* GetWindow();
	SDL_Surface* GetScreenSurface();

private:
	/* CONFIGURATION VARIABLES */
	WindowType windowType;
	SDL_Window* window			= NULL; //The window we'll be rendering to
	SDL_Surface* screenSurface	= NULL; //The surface contained by the window
};
