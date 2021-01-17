#pragma once
#include "UIComponent.h"
#include <deque>
#include <list>
#include <vector>
#include "imgui.h"

class UIConfiguration : public UIComponent
{
public:
	UIConfiguration();
	~UIConfiguration();

	void Draw();

private:
	const static unsigned short MAX_FRAMES_STORAGE	= 60;

	ImVec4 purple									= ImVec4(1.0f, 0.0f, 1.0f, 1.0f);

	std::vector<float> frames;
	std::vector<float> millis;

	void AddFrame(float fps);
	void AddMillis(float frameMillis);
};

