#pragma once
#include "UIComponent.h"
class UISceneView : public UIComponent
{
public:
    void Draw() override;

    int GetSceneWidth() const { return width; }
    int GetSceneHeight() const { return height; }
private:
    int width = 0;
    int height = 0;
};

