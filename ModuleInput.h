#pragma once
#include "Module.h"
#include "Point.h"
#include "Globals.h"
#include "Math/float2.h"

#include "SDL/include/SDL_scancode.h"

typedef unsigned __int8 Uint8;

#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};


class ModuleInput : public Module
{
public:

	enum EventWindow
	{
		WE_QUIT = 0,
		WE_HIDE = 1,
		WE_SHOW = 2,
		WE_RESIZED = 3,
		WE_COUNT
	};
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	KeyState GetKey(int id) const { return keyboard[id]; }
	KeyState GetMouseButtonDown(int id) const { return mouseButtons[id - 1]; }
	bool GetWindowEvent(EventWindow ev) const { return windowEvents[ev]; }
	const float GetMouseWheelMotion() const { return mouseWheelMotion; }
	const float2& GetMouseMotion() const { return mouseMotion; }

private:
	//const Uint8 *keyboard = NULL;
	bool		windowEvents[WE_COUNT];
	KeyState	keyboard[MAX_KEYS];
	KeyState	mouseButtons[NUM_MOUSE_BUTTONS];

	float2 mouseMotion = { 0, 0 };
	float2 mouse = { 0, 0 };
	float mouseWheelMotion = 0;
};
