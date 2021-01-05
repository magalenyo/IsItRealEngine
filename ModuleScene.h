#pragma once
#include "Module.h"

class GameObject;

class ModuleScene : public Module
{
public:
    bool Init() override;

private:
    GameObject* root = nullptr;
};

