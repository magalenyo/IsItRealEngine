#pragma once
#include "UIComponent.h"
#include "imgui.h"
#include "Math/float2.h"

class UISceneView : public UIComponent
{
public:
    void Draw() override;

    float2 GetMousePos() const;

private:
    ImVec2 mousePos;
};

