#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"
#include "Math/float3.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:


	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void* GetContext();
	unsigned int GetDefaultProgram();
	void LoadModel(const char* path);

	void SetGridColor(float3 newColor);
	float3 GetGridColor() const;

	bool& GetAxisState();
	bool& GetGridState();
	bool& GetModelState();

	void TurnAxis(bool state);
	void TurnGrid(bool state);

private:
	
	void* context;
	unsigned int defaultProgram;
	Model* loadedModel = nullptr;

	/* CONFIGURATION VARIABLES */
	float3 gridColor;
	bool activateAxis = true;
	bool activateGrid = true;
	bool activatedModel = true;

	void RenderAxis();
	void RenderGrid();
	void RenderModel();
};
