#pragma once

#include "GameObject.h"
#include "Globals.h"
#include "Geometry/AABB.h"
#include <vector>
#include "assimp/scene.h"

class QTNode
{
	friend class Quadtree;

public:
	QTNode(AABB& surface);
	void AddGameObject(GameObject* gameObject);

	template<class GEOMETRY> std::vector<GameObject*> GetObjectsCollided(GEOMETRY geometry);

	AABB GetSurface() { return surface; }

private:
	AABB surface;
	unsigned int maxGObjectsInNode = 2;
	std::vector<GameObject*> GObjectsInNode;
	std::vector<QTNode> childNodes;

	void Subdivide();
	void Restructure();
	void Draw();
	void Clear();
};

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void AddGameObject(GameObject* gameObject);
	bool CleanUp();
	void Draw();
	void Clear();

	template<class GEOMETRY> 
	std::vector<GameObject*> GetObjectsCollided(GEOMETRY geometry);
	

private:
	QTNode* root = nullptr;
};

template<class GEOMETRY> 
std::vector<GameObject*> Quadtree::GetObjectsCollided(GEOMETRY geometry)
{
	return root->GetObjectsCollided(geometry);
}

template<class GEOMETRY> 
std::vector<GameObject*> QTNode::GetObjectsCollided(GEOMETRY geometry)
{
	std::vector<GameObject*> GObjectsCollided;
	if (geometry.Intersects(surface))
	{
		for(unsigned int i = 0; i < GObjectsInNode.size(); ++i)
		{
			GObjectsCollided.push_back(GObjectsInNode[i]);
		}
		for (unsigned int i = 0; i < childNodes.size(); ++i)
		{
			std::vector<GameObject*> GObjectsCollidedFromChilds = childNodes[i].GetObjectsCollided(geometry);
			if (!GObjectsCollidedFromChilds.empty())
			{
				GObjectsCollided.insert(GObjectsCollided.end(), GObjectsCollidedFromChilds.begin(), GObjectsCollidedFromChilds.end());
			}			
		}		
	}
	return GObjectsCollided;
}
