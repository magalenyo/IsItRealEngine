#pragma once

#include "assimp/mesh.h"
#include <vector>
#include "Math/float3.h"
#include "Math/float4x4.h"
#include "Geometry/Triangle.h"
#include "Component.h"
#include "ComponentMaterial.h"

class GameObject;

class ComponentMesh : public Component
{
public:
	ComponentMesh(const aiMesh* mesh, GameObject* owner);
	~ComponentMesh();

	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	void SetMaterialIndex(unsigned int newMaterialIndex);
	void Draw(const std::vector<ComponentMaterial*>& materials, const float4x4& modelMatrix);
	void RenderToEditor() override;
	void SetSerializedName(const std::string& path);

	bool CleanUp();

	unsigned int GetMaterialIndex() const;
	unsigned int GetNumVertices() const;
	unsigned int GetNumIndices() const;
	unsigned int GetTriangles() const;
	unsigned int GetFaces() const;
	std::vector<Triangle> GetVectorTriangles() const;
	float3 GetFurthestPosition();
	std::string GetSerializedName() const;

	void Serialize(Value& value, Document::AllocatorType& allocator) override;

private:
	static const int BIND_VERTEX_ARRAY_END = 0;

	unsigned int materialIndex = 0;
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;
	unsigned int numFaces = 0;

	unsigned int vbo;
	unsigned int ebo;
	unsigned int vao;

	std::vector<float3> totalVertices;
	std::vector<Triangle> triangles;

	float3 furthestPosition = float3(0, 0, 0);

	std::string serializedName = "";

	void DestroyBuffer(unsigned int buffer);
};

