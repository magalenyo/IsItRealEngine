#include "Mesh.h"
#include "GL/glew.h"
#include "Math/float2.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	//unsigned buffer_size = vertex_size * mesh->mNumVertices;

	//// glBufferData has nullptr because we don't have the buffer yet and we'll do it in bufferSubData
	//glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	//unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	//// a mesh can contain an array of arrays of UVs
	//glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	//unsigned uv_offset = position_size;
	//unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	//float2* uvs = (float2*) (glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	//for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	//{
	//	uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	//}
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	//numVertices = mesh->mNumVertices;
}
