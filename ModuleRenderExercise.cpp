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

bool ModuleRenderExercise::Init()
{


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
	//glBindTexture(GL_TEXTURE_2D, App->textures->GetTextureID());
	glUniform1i(glGetUniformLocation(programTexture, "mytexture"), 0);
	glUseProgram(programTexture);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);

}