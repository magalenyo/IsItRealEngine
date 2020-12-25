#include "Mesh.h"
#include "GL/glew.h"
#include "Math/float2.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "Math/float3.h"

#include "MemoryLeakDetector.h"

Mesh::Mesh()
{
}

Mesh::Mesh(const aiMesh* mesh)
{
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
	SetMaterialIndex(mesh->mMaterialIndex);
}

Mesh::~Mesh()
{
	CleanUp();
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// float, float, float -> v0
	// uv, uv			   -> uv0
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	float* vertices = (float*)(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	float3 currentPosition = float3(0, 0, 0);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{

		*(vertices++) = mesh->mVertices[i].x;
		*(vertices++) = mesh->mVertices[i].y;
		*(vertices++) = mesh->mVertices[i].z;
		*(vertices++) = mesh->mTextureCoords[0][i].x;
		*(vertices++) = mesh->mTextureCoords[0][i].y;

		if (mesh->mVertices[i].z > currentPosition.z) {
			currentPosition = float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		}
	}
	furthestPosition = currentPosition;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	numVertices = mesh->mNumVertices;
}


void Mesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	// if it's GL_MAP_WRITE_BIT it won't let it load, needs to be READ_ONLY
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	numIndices = mesh->mNumFaces * 3;
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 3));
	// needs to be called 0 in order to "close" the glBindVertexArray(vao)
	glBindVertexArray(BIND_VERTEX_ARRAY_END);
}

void Mesh::CreateFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

}

void Mesh::SetMaterialIndex(unsigned int newMaterialIndex)
{
	materialIndex = newMaterialIndex;
}

void Mesh::Draw(const std::vector<unsigned>& model_textures)
{
	unsigned program = App->renderer->GetDefaultProgram();
	const float4x4& view = App->camera->GetViewMatrix();
	const float4x4& proj = App->camera->GetProjectionMatrix();
	float4x4 model = float4x4::identity;

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[materialIndex]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

bool Mesh::CleanUp()
{
	DestroyBuffer(fbo);
	DestroyBuffer(vao);	// last created, first deleted
	DestroyBuffer(ebo);
	DestroyBuffer(vbo);
	return true;
}

int Mesh::GetNumVertices() const
{
	return numVertices;
}

int Mesh::GetNumIndices() const
{
	return numIndices;
}

float3 Mesh::GetFurthestPosition()
{
	return furthestPosition;
}

void Mesh::DestroyBuffer(unsigned int buffer)
{
	glDeleteBuffers(1, &buffer);
}
