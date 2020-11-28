#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define OWN_PI 3.14159265359
#define DEGTORAD OWN_PI/180
#define RADTODEG 180/OWN_PI

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define FULLSCREEN_DESKTOP false
#define WINDOW_BORDERLESS false
#define WINDOW_RESIZABLE true
#define VSYNC true
#define TITLE "IsItRealEngine"
