#pragma once
#include "UIComponent.h"
#include "Globals.h"

#include "imgui.h"

class UIConsole : public UIComponent
{
public:
	UIConsole();
	~UIConsole();

	void Draw();
	void ClearLog();
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);
private:
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    int                   defaultWidth = SCREEN_WIDTH;
    int                   defaultHeight = 120;
};

