#include "ComponentMesh.h"
#include "GL/glew.h"
#include "Math/float2.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "Math/float3.h"

#include "MemoryLeakDetector.h"

ComponentMesh::ComponentMesh()
{
}

ComponentMesh::ComponentMesh(const aiMesh* mesh)
{
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
	SetMaterialIndex(mesh->mMaterialIndex);
}

ComponentMesh::~ComponentMesh()
{
	CleanUp();
}

void ComponentMesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// float, float, float -> v0
	// uv, uv			   -> uv0
	unsigned vertexSize = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned bufferSize = vertexSize * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
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
	numFaces = mesh->mNumFaces;
}


void ComponentMesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned indexSize = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, nullptr, GL_STATIC_DRAW);
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

void ComponentMesh::CreateVAO()
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

void ComponentMesh::SetMaterialIndex(unsigned int newMaterialIndex)
{
	materialIndex = newMaterialIndex;
}

void ComponentMesh::Draw(const std::vector<unsigned>& model_textures)
{
	unsigned program = App->renderer->GetDefaultProgram();
	const float4x4& view = App->camera->GetViewMatrix();
	const float4x4& proj = App->camera->GetProjectionMatrix();
	//float4x4 model = float4x4::identity;
	float4x4 model = float4x4(1, 0, 0, 10,
							  0, 3, 0, 0,
							  0, 0, 1, 0,
						      0, 0, 0, 1);

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[materialIndex]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(BIND_VERTEX_ARRAY_END);
}

bool ComponentMesh::CleanUp()
{
	DestroyBuffer(vao);	// last created, first deleted
	DestroyBuffer(ebo);
	DestroyBuffer(vbo);
	return true;
}

unsigned int ComponentMesh::GetNumVertices() const
{
	return numVertices;
}

unsigned int ComponentMesh::GetNumIndices() const
{
	return numIndices;
}

unsigned int ComponentMesh::GetTriangles() const
{
	return numVertices / 3;
}

unsigned int ComponentMesh::GetFaces() const
{
	return numFaces;
}

float3 ComponentMesh::GetFurthestPosition()
{
	return furthestPosition;
}

void ComponentMesh::DestroyBuffer(unsigned int buffer)
{
	glDeleteBuffers(1, &buffer);
}