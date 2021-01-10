#pragma once

#include "GameObject.h"
#include "Globals.h"
#include "Geometry/AABB.h"
#include <vector>

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void AddGameObject(const GameObject* gameObject);
	bool CleanUp();
	void Draw();

	template<typename GEOMETRY> std::vector<GameObject*> GetObjectsCollided(GEOMETRY geometry);
	

private:
	QTNode* root = nullptr;
};

template<typename GEOMETRY> std::vector<GameObject*> Quadtree::GetObjectsCollided(GEOMETRY geometry)
{
	return root->GetObjectsCollided(geometry);
}

class QTNode
{
	friend class Quadtree;

public:
	QTNode(AABB& surface);
	void AddGameObject(const GameObject* gameObject);

	template<typename GEOMETRY> std::vector<GameObject*> GetObjectsCollided(GEOMETRY geometry);

	AABB GetSurface() { return surface; }

private:
	AABB surface;
	unsigned int maxGObjectsInNode = 2;
	std::vector<const GameObject*> GObjectsInNode;
	std::vector<QTNode> childNodes;

	void Subdivide();
	void Restructure();
	void Draw();
};

template<typename GEOMETRY> std::vector<GameObject*> QTNode::GetObjectsCollided(GEOMETRY geometry)
{
	if (geometry.Intersects(cube)) 
	{
		vector<GameObject*> GObjectsCollided;
		for(unsigned int i = 0; i < GObjectsInNode.size(); ++i)
		{
			GObjectsCollided.push_back(GObjectsInNode[i]);
		}
		for (unsingeed int i = 0; i < childNodes.size(); ++i)
		{
			vector<GameObject*> GObjectsCollidedFromChilds = childNodes[i].GetObjectsCollided(geometry);
			GObjectsCollided.insert(GObjectsCollided.end(), GObjectsCollidedFromChilds.begin(), GObjectsCollidedFromChilds.end());
		}
		return GObjectsCollided;
	}
}