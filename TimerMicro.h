#pragma once

#include "SDL.h"

class TimerMicro
{
public:
	TimerMicro() {}
	~TimerMicro() {}


	void Start() {
		stopped = false;
		lastCycle = SDL_GetPerformanceCounter();
		currentCycle = SDL_GetPerformanceCounter();
	}

	double Read() {
		if (!stopped) {
			currentCycle = SDL_GetPerformanceCounter();
		}
		return ((double) (currentCycle - lastCycle)) / performanceFrequency;
	}

	double Stop() {
		stopped = true;
		return Read();
	}


private:
	unsigned int currentCycle, lastCycle;
	Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	bool stopped;
};

