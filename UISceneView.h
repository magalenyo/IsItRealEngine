#pragma once
#include "UIComponent.h"
#include "imgui.h"
#include "Math/float2.h"

class UISceneView : public UIComponent
{
public:
    void Draw() override;

    float2 GetMousePos() const;
    float2 GetViewportPos() const;

private:
    ImVec2 mousePos;
    ImVec2 viewportPos;

    //float2 framebuffer_position = { 0.0f, 0.0f };
    float2 framebuffer_size = { 0.0f, 0.0f };
};

