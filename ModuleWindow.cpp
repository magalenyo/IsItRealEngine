#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "MemoryLeakDetector.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		// DEFAULT SETTINGS
		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			windowType = WindowType::W_FULLSCREEN;
		}

		if (FULLSCREEN_DESKTOP == true) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			windowType = WindowType::W_FULLSCREEN_DESKTOP;
		}

		if (WINDOW_BORDERLESS == true) {
			flags |= SDL_WINDOW_BORDERLESS;
			windowType = WindowType::W_WINDOW_BORDERLESS;
		}

		if (WINDOW_RESIZABLE == true) {
			flags |= SDL_WINDOW_RESIZABLE;
			windowType = WindowType::W_WINDOW_RESIZABLE;
		}

		flags |= SDL_RENDERER_PRESENTVSYNC;		// this for vsync

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		//SDL_GL_SetSwapInterval(0);				// or this
		TurnVSYNC(false);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

update_status ModuleWindow::Update()
{
	SetWindowConfiguration();
	return UPDATE_CONTINUE;
}

int ModuleWindow::GetWindowWidth() const
{
	int w;
	int h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	
	return w;
}

int ModuleWindow::GetWindowHeight() const
{
	int w;
	int h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	return h;
}

void ModuleWindow::GetWindowSize(int &w, int &h) const
{
	SDL_GetWindowSize(App->window->window, &w, &h);
}

void ModuleWindow::TurnVSYNC(bool param)
{
	SDL_GL_SetSwapInterval(param);
}

void ModuleWindow::SetWindowConfiguration()
{
	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	switch (windowType){
		case WindowType::W_FULLSCREEN :
			flags |= SDL_WINDOW_FULLSCREEN;
			SDL_SetWindowFullscreen(window, flags);	// SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0;
			break;
		case WindowType::W_FULLSCREEN_DESKTOP:
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;	
			SDL_SetWindowFullscreen(window, flags); // SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0;
			break;
		case WindowType::W_WINDOW_BORDERLESS:
			flags |= SDL_WINDOW_BORDERLESS;
			SDL_SetWindowFullscreen(window, flags);
			SDL_SetWindowBordered(window, SDL_FALSE); // False to remove border
			break;
		case WindowType::W_WINDOW_RESIZABLE:
			flags |= SDL_WINDOW_RESIZABLE;
			SDL_SetWindowFullscreen(window, flags);
			SDL_SetWindowBordered(window, SDL_TRUE);
			break;
	}
	
}

int ModuleWindow::GetWindowType()
{
	return static_cast<int>(windowType);
}

void ModuleWindow::SetWindowType(int type)
{
	windowType = static_cast<ModuleWindow::WindowType>(type);
}

