#pragma once
#include "UIComponent.h"
#include <deque>
#include <list>
#include <vector>

class UIConfiguration : public UIComponent
{
public:
	UIConfiguration();
	~UIConfiguration();

	void Draw();

private:
	const static unsigned short MAX_FRAMES_STORAGE = 60;

	std::vector<float> frames;
	std::vector<float> millis;

	void AddFrame(float fps);
	void AddMillis(float frameMillis);
};

