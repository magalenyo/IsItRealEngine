#include "FSMesh.h"
#include "ComponentMesh.h"
#include <iostream>
#include <fstream>
#include "Application.h"
#include "SceneImporter.h"


void FSMesh::ExportMesh(const aiMesh* mesh, const std::string& path)
{
	// vertices, normals, uvs, faces
	unsigned int header[2] = { mesh->mNumVertices, mesh->mNumFaces };

	unsigned int headerSize = sizeof(header);
	// vertice position + vertice normal + UV
	unsigned int vertexSize = (sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 2) * mesh->mNumVertices;
	unsigned int indexSize = sizeof(unsigned int) * mesh->mNumFaces * 3;

	unsigned int bufferSize = headerSize + vertexSize + indexSize;

	char* fileBuffer = new char[bufferSize];
	char* cursor = fileBuffer;
	unsigned int bytes = headerSize; // First store ranges
	memcpy(cursor, header, bytes);
	cursor += bytes;

	bytes = sizeof(float);
	
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{

		//memcpy(cursor, &((float) mesh->mVertices[i].x), bytes); -> might do more work
		*((float*)cursor) = (float) mesh->mVertices[i].x;	
		cursor += bytes;
		*((float*)cursor) = (float)mesh->mVertices[i].y;
		cursor += bytes;
		*((float*)cursor) = (float)mesh->mVertices[i].z;
		cursor += bytes;

		*((float*)cursor) = (float)mesh->mNormals[i].x;
		cursor += bytes;
		*((float*)cursor) = (float)mesh->mNormals[i].y;
		cursor += bytes;
		*((float*)cursor) = (float)mesh->mNormals[i].z;
		cursor += bytes;

		aiVector3D* textureVec = mesh->mTextureCoords[0];
		if (textureVec != nullptr) {
			*((float*)cursor) = (float)textureVec[i].x;
			cursor += bytes;
			*((float*)cursor) = (float)textureVec[i].y;
			cursor += bytes;
		}
		else {
			*((float*)cursor) = (float)0.0f;
			cursor += bytes;
			*((float*)cursor) = (float)0.0f;
			cursor += bytes;
		}
		
	}
	bytes = sizeof(unsigned int);
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		if (mesh->mFaces[i].mNumIndices == 3) {
			*((unsigned int*)cursor) = mesh->mFaces[i].mIndices[0];
			cursor += bytes;
			*((unsigned int*)cursor) = mesh->mFaces[i].mIndices[1];
			cursor += bytes;
			*((unsigned int*)cursor) = mesh->mFaces[i].mIndices[2];
			cursor += bytes;
		}
		else if (mesh->mFaces[i].mNumIndices == 2) {
			*((unsigned int*)cursor) = mesh->mFaces[i].mIndices[0];
			cursor += bytes;
			*((unsigned int*)cursor) = mesh->mFaces[i].mIndices[1];
			cursor += bytes;
			*((unsigned int*)cursor) = 0;
			cursor += bytes;
		}
	}

	std::ofstream file;
	file.open(path);
	if (file.is_open()) {
		size_t len = bufferSize;
		file.write(fileBuffer, len);
		file.close();
	}
	
}

ComponentMesh* FSMesh::ImportMaterial(std::string path)
{

	return nullptr;
}


// https://stackoverflow.com/questions/15313658/void-is-literally-float-how-to-cast -> reverse
// https://www.youtube.com/watch?v=K2bqrLsQgnI ->memcpy needs
// para el xport, fileBuffer es input
//char* cursor2 = fileBuffer;
//unsigned int header2[2];
//unsigned int bytes2 = sizeof(header2);
//memcpy(header2, cursor2, bytes2);
//cursor2 += bytes2;
//float possibleFloat = *((float*)cursor2);
//cursor2 += bytes2;