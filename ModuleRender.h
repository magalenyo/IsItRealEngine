#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"
#include "Math/float3.h"
#include "Geometry/AABB.h"
#include "Geometry/OBB.h"

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
	
	void SetGridColor(float3 newColor);
	float3 GetGridColor() const;

	bool& GetAxisState();
	bool& GetGridState();
	bool& GetModelState();
	bool& GetGLDepthTestState();
	bool& GetGLCullFaceState();
	bool& GetDrawQuadtreeState();
	
	unsigned int GetSceneFBO() { return sceneFBO; }
	unsigned int GetSceneTexture() { return sceneTexture; }
	unsigned int GetMissingTexture() const { return missingTextureID; }

	void TurnAxis(bool state);
	void TurnGrid(bool state);

	void OnSceneResize(int width, int height);
	
	void AddAABBQuadtree(AABB aabb); //Adds an AABB to a vector that represents the drawable part of the quadtree.
	

public:
	unsigned int viewportWidth	= 0;
	unsigned int viewportHeight = 0;

private:
	
	void* context;
	unsigned int defaultProgram;
	unsigned int sceneFBO		= 0;
	unsigned int sceneRBO		= 0;
	unsigned int sceneTexture	= 0;
	unsigned int missingTextureID;			// This texture is used when there is no texture for a Mesh

	std::vector<AABB> aabbsQuadtree;
	
	/* CONFIGURATION VARIABLES */
	float3 gridColor			= { 1.000000f, 1.000000f, 1.000000f };
	bool activeAxis				= true;
	bool activeGrid				= true;
	bool activeModel			= true;
	bool activeGLDepthTest		= true;
	bool activeGLCullFace		= true;
	bool activeDrawQuadtree		= false;
	
	void LoadRenderConfiguration();
	void RenderAxis();
	void RenderGrid();
	void RenderBoxes();
};