#pragma once

#include "GameObject.h"
#include "Globals.h"
#include "Geometry/AABB.h"
#include <vector>
#include <map>
#include "assimp/scene.h"

class QTNode
{
	friend class Quadtree;

public:
	QTNode(AABB& surface);
	void AddGameObject(GameObject* gameObject);

	template<class GEOMETRY> 
	std::vector<GameObject*> GetObjectsCollided(GEOMETRY geometry);
	template<class GEOMETRY>
	void GetObjectsCollided(std::map<float, GameObject*>& GObjectsCollided, GEOMETRY geometry);

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
	template<class GEOMETRY>
	void GetObjectsCollided(std::map<float, GameObject*>& GObjectsCollided, GEOMETRY geometry);
	

private:
	QTNode* root = nullptr;
};

template<class GEOMETRY> 
std::vector<GameObject*> Quadtree::GetObjectsCollided(GEOMETRY geometry)
{
	if (root != nullptr)
	{
		return root->GetObjectsCollided(geometry);
	}
}

template<class GEOMETRY>
void Quadtree::GetObjectsCollided(std::map<float, GameObject*>& GObjectsCollided, GEOMETRY geometry)
{
	if (root != nullptr)
	{
		root->GetObjectsCollided(GObjectsCollided, geometry);
	}
}

template<class GEOMETRY> 
std::vector<GameObject*> QTNode::GetObjectsCollided(GEOMETRY geometry)
{
	std::vector<GameObject*> GObjectsCollided;
	if (geometry.Intersects(surface))
	{
		for(GameObject* go : GObjectsInNode)
		{
			GObjectsCollided.push_back(go);
		}
		for (QTNode child : childNodes)
		{
			std::vector<GameObject*> GObjectsCollidedFromChilds = child.GetObjectsCollided(geometry);
			if (!GObjectsCollidedFromChilds.empty())
			{
				GObjectsCollided.insert(GObjectsCollided.end(), GObjectsCollidedFromChilds.begin(), GObjectsCollidedFromChilds.end());
			}
		}		
	}
	return GObjectsCollided;
}

template<class GEOMETRY>
void QTNode::GetObjectsCollided(std::map<float, GameObject*>& GObjectsCollided, GEOMETRY geometry)
{
	if (geometry.Intersects(surface))
	{
		float hitNear, hitFar;
		for (GameObject* go : GObjectsInNode)
		{
			if (geometry.Intersects(go->GetAABB(), hitNear, hitFar))
			{
				GObjectsCollided[hitNear] = go;
			}
		}
		for (QTNode child : childNodes)
		{
			child.GetObjectsCollided(GObjectsCollided, geometry);
		}
	}
}
