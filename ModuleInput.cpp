#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "imgui_impl_sdl.h"
#include "MemoryLeakDetector.h"



ModuleInput::ModuleInput()
{
	/*memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);*/
}

// Destructor
ModuleInput::~ModuleInput()
{

}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	memset(windowEvents, false, WE_COUNT * sizeof(bool));
	mouseMotion = { 0, 0 };
	mouseWheelMotion = 0;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
			}
			else
			{
				keyboard[i] = KEY_REPEAT;
			}
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
			}
			else
			{
				keyboard[i] = KEY_IDLE;
			}
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
		{
			mouseButtons[i] = KEY_REPEAT;
		}

		if (mouseButtons[i] == KEY_UP)
		{
			mouseButtons[i] = KEY_IDLE;
		}
	}

	while (SDL_PollEvent(&event) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				App->camera->OnWindowResized(event.window.data1, event.window.data2);
				windowEvents[WE_RESIZED] = true;		// this might not be needed
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouseMotion.x = event.motion.xrel / 2;
			mouseMotion.y = event.motion.yrel / 2;
			mouse.x = event.motion.x / 2;
			mouse.y = event.motion.y / 2;
			
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
				mouseWheelMotion = event.wheel.x;
			}
			else {
				mouseWheelMotion = event.wheel.y;
			}
			break;
		case SDL_DROPFILE:
			char* dropped_filedir = event.drop.file;
			std::string dropped_file = event.drop.file;
			App->scene->LoadDroppedFile(dropped_file);
			SDL_free(dropped_filedir);
		
			break;
		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
