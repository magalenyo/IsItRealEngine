#pragma once

#include "SDL.h"

class TimerMili
{
public:
	
	TimerMili() {}
	~TimerMili() {}


	void Start() {
		stopped = false;
		currentTime = lastTime = SDL_GetTicks();
	}

	int Read() {
		if (!stopped) {
			currentTime = SDL_GetTicks();
		}
		return currentTime - lastTime;
	}
	int Stop() {
		stopped = true;
		return Read();
	}
	

private:
	unsigned int lastTime = 0, currentTime;
	bool stopped = false;
};