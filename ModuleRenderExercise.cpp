#include "ModuleRenderExercise.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "Globals.h"
#include "GL/glew.h"
#include "SDL.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}

bool ModuleRenderExercise::Init()
{
#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,	true);
#endif

	// Inits the triangle shaders
    CreateTriangleVBO();
    unsigned vertexShader = App->program->CompileShader(GL_VERTEX_SHADER, "./shaders/VertexShader.glsl");
    unsigned fragmentShader = App->program->CompileShader(GL_FRAGMENT_SHADER, "./shaders/FragmentShader.glsl");
    program = App->program->CreateProgram(vertexShader, fragmentShader);

	// Inits the texture shaders
	App->textures->LoadTexture("./resources/lenna.png");
	CreateTriangleVBOTexture();
	unsigned vertexShaderTexture = App->program->CompileShader(GL_VERTEX_SHADER, "./shaders/VertexShaderTexture.glsl");
	unsigned fragmentShaderTexture = App->program->CompileShader(GL_FRAGMENT_SHADER, "./shaders/FragmentShaderTexture.glsl");
	programTexture = App->program->CreateProgram(vertexShaderTexture, fragmentShaderTexture);

	return true;
}

bool ModuleRenderExercise::CleanUp()
{
    DestroyVBO(vbo);
	DestroyVBO(vboTexture);
    return true;
}

update_status ModuleRenderExercise::Update()
{
	//RenderVBO(vbo);
	RenderVBOTexture(vboTexture);
	return UPDATE_CONTINUE;
}

unsigned ModuleRenderExercise::CreateTriangleVBO()
{
    float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

    return vbo;
}

unsigned ModuleRenderExercise::CreateTriangleVBOTexture()
{
	float buffer_data[] = {
		-0.5f, 0.0f, 0.0f, //  v0 pos
		 0.5f, 0.0f, 0.0f, // v1 pos
		-0.5f, 1.0f, 0.0f, //  v2 pos

		 0.5f, 0.0f, 0.0f, //  v3 pos
		 0.5f, 1.0f, 0.0f, // v4 pos
		-0.5f, 1.0f, 0.0f, //  v5 pos

		0.0f, 0.0f, //  v0 texcoord
		1.0f, 0.0f, //  v1 texcoord
		0.0f, 1.0f, //  v2 texcoord

		1.0f, 0.0f, //  v3 texcoord
		1.0f, 1.0f, //  v4 texcoord
		0.0f, 1.0f //  v5 texcoord
	};

	glGenBuffers(1, &vboTexture);
	glBindBuffer(GL_ARRAY_BUFFER, vboTexture); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);

	return vbo;
}

void ModuleRenderExercise::DestroyVBO(unsigned _vbo)
{
    glDeleteBuffers(1, &_vbo);
}

void ModuleRenderExercise::RenderVBO(unsigned _vbo)
{
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glEnableVertexAttribArray(0);
	//// size = 3 float per vertex
	//// stride = 0 is equivalent to stride = sizeof(float)*3
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//// 1 triangle to draw = 3 vertices
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void ModuleRenderExercise::RenderVBOTexture(unsigned _vbo)
{

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*) (sizeof(float) * 6 * 3) // buffer offset
	);

	float4x4 model = float4x4::identity;
	float4x4 view = App->camera->GetViewMatrix();
	float4x4 proj = App->camera->GetProjectionMatrix();
	
	glUniformMatrix4fv(glGetUniformLocation(programTexture, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programTexture, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programTexture, "proj"), 1, GL_TRUE, &proj[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->textures->GetTextureID());
	glUniform1i(glGetUniformLocation(programTexture, "mytexture"), 0);
	glUseProgram(programTexture);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);

}