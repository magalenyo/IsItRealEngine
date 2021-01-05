#include "ModuleScene.h"
#include "GameObject.h"


bool ModuleScene::Init()
{
	root = new GameObject();
	return true;
}
