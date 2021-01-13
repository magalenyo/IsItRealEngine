#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include "GL/glew.h"
#include "Geometry/Frustum.h"
#include "DebugLogger.h"

#include "Math/float4x4.h"
#include "ModuleDebugDraw.h"
#include "debugdraw.h" 
#include "Math/float3.h"

#include "Assimp/cimport.h" // aiLogStream

#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "UISceneView.h"

#include "MemoryLeakDetector.h"


ModuleRender::ModuleRender()
{

}

// Destructor
ModuleRender::~ModuleRender()
{
	glDeleteTextures(1, &missingTextureID);
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_CreateContext(App->window->GetWindow());


	GLenum err = glewInit();
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE);	 // Enable cull backward faces
	glFrontFace(GL_CCW);	 // Front faces will be counter clockwise

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&DebugLogger::GLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif

	// This is to output the log from Assimp
	struct aiLogStream stream;
	stream.callback = DebugLogger::AssimpErrorFunction;
	aiAttachLogStream(&stream);
	// *************************

	unsigned vertexShaderTexture = App->program->CompileShader(GL_VERTEX_SHADER, "./shaders/VertexShaderTexture.glsl");
	unsigned fragmentShaderTexture = App->program->CompileShader(GL_FRAGMENT_SHADER, "./shaders/FragmentShaderTexture.glsl");
	defaultProgram = App->program->CreateProgram(vertexShaderTexture, fragmentShaderTexture);

	//loadedModel = new Model("./resources/models/BakerHouse.fbx");

	gridColor = { 1.000000f, 1.000000f, 1.000000f };

	missingTextureID = App->textures->LoadTexture("./resources/textures/missing_texture.png");
	if (missingTextureID == ModuleTexture::TEXTURE_ERROR) {
		missingTextureID = 0;
	}
	
	// generate texture
	glGenTextures(1, &sceneTexture);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 400, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a renderbuffer object to store depth info
	glGenRenderbuffers(1, &sceneRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, sceneRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 400, 400);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// create a framebuffer object
	glGenFramebuffers(1, &sceneFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	// attach the texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
						   GL_COLOR_ATTACHMENT0,  // 2. attachment point
						   GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
						   sceneTexture,          // 4. tex ID
						   0);                    // 5. mipmap level: 0(base)

	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
							  GL_DEPTH_ATTACHMENT, // 2. attachment point
							  GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
							  sceneRBO);              // 4. rbo ID

	// switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("ERROR: FrameBuffer incomplete");
	}

	return true;
}

update_status ModuleRender::PreUpdate()
{
	// set rendering destination to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	glViewport(0, 0, viewportWidth, viewportHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	LoadRenderConfiguration();
	RenderAxis();
	RenderGrid();
	App->debugDraw->Draw(App->camera->GetCamera()->GetViewMatrix(), App->camera->GetCamera()->GetProjectionMatrix(), viewportWidth, viewportHeight);
	//RenderModel();

	// unbind FBO
	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	// trigger mipmaps generation explicitly
	// NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
	// triggers mipmap generation automatically. However, the texture attached
	// onto a FBO should generate mipmaps manually via glGenerateMipmap().
	//TODO: Revisar esto
	/*glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);*/


	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SDL_GL_SwapWindow(App->window->GetWindow());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");
	if (loadedModel != nullptr) {
		delete loadedModel;
	}
	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void* ModuleRender::GetContext()
{
	return context;
}

unsigned int ModuleRender::GetDefaultProgram()
{
	return defaultProgram;
}

void ModuleRender::LoadModel(std::string path)
{
	LOG("File %s dropped", path.c_str());
	if (ModuleTexture::IsTexture(path)) {
		LOG("File %s is a TEXTURE", path.c_str());
		loadedModel->LoadSingleTexture(path);
	}
	else if (Model::CanLoadFBX(path)){
		LOG("File %s is a MODEL", path.c_str());
		delete loadedModel;
		loadedModel = nullptr;
		loadedModel = new Model(path.c_str());
	}
	else {
		LOG("%s cannot be loaded", path.c_str());
	}
}

void ModuleRender::SetGridColor(float3 newColor)
{
	gridColor = newColor;
}

float3 ModuleRender::GetGridColor() const
{
	return gridColor;
}

Model* ModuleRender::GetModel() const
{
	return loadedModel;
}

bool& ModuleRender::GetAxisState()
{
	return activeAxis;
}

bool& ModuleRender::GetGridState()
{
	return activeGrid;
}

bool& ModuleRender::GetModelState()
{
	return activeModel;
}

bool& ModuleRender::GetGLDepthTestState()
{
	return activeGLDepthTest;
}

bool& ModuleRender::GetGLCullFaceState()
{
	return activeGLCullFace;
}

void ModuleRender::TurnAxis(bool state)
{
	activeAxis = state;
}

void ModuleRender::TurnGrid(bool state)
{
	activeGrid = state;
}

void ModuleRender::OnSceneResize(int width, int height)
{

	viewportWidth = width;
	viewportHeight = height;

	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, sceneRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG("ERROR: FrameBuffer incomplete");
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRender::RenderAxis()
{
	if (activeAxis) {
		dd::axisTriad(float4x4::identity, 0.1f, 2.0f);
	}
}

void ModuleRender::RenderGrid()
{
	if (activeGrid) {
		dd::xzSquareGrid(-25, 25, 0.0f, 1.0f, gridColor);
	}
}

void ModuleRender::RenderModel()
{
	if (activeModel) {
		loadedModel->Draw();
	}
}

void ModuleRender::LoadRenderConfiguration()
{
	if (activeGLCullFace) {
		glEnable(GL_CULL_FACE); // Enable cull backward faces
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (activeGLDepthTest) {
		glEnable(GL_DEPTH_TEST); // Enable depth test
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

