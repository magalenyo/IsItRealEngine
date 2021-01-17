#include "Quadtree.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRender.h"

Quadtree::Quadtree()
{
	root = new QTNode(AABB(vec(-100, -50, -100), vec(100, 50, 100)));
}

Quadtree::~Quadtree()
{
	if (root != nullptr)
	{
		root->Clear();
		delete root;
		root = nullptr;
	}
}

void Quadtree::AddGameObject(GameObject* gameObject)
{
	if (root != nullptr)
	{
		if (gameObject->GetAABB().Intersects(root->GetSurface()))
		{
			root->AddGameObject(gameObject);
		}
	}
}

bool Quadtree::CleanUp()
{
	root->GObjectsInNode.clear();
	root->childNodes.clear();
	return true;
}

void Quadtree::Draw()
{
	root->Draw();
}

void Quadtree::Clear()
{
	root->Clear();
}

QTNode::QTNode(AABB& surface) : surface(surface) {}

void QTNode::AddGameObject(GameObject* gameObject)
{
	if (childNodes.empty() && GObjectsInNode.size() < maxGObjectsInNode) 
	{
		GObjectsInNode.push_back(gameObject);
		
	}
	else
	{
		if (childNodes.empty())
		{
			Subdivide();
		}
		GObjectsInNode.push_back(gameObject);
		Restructure();
	}
}

void QTNode::Subdivide()
{
	/* 
	* [0][1] Child nodes positions
	* [2][3]
	*/
	const float3 surfaceSize = surface.Size();
	const float3 subdividedSurfaceSize = { surfaceSize.x / 2, surfaceSize.y, surfaceSize.z / 2 };
	const float3 centerSurface = surface.CenterPoint();
	float3 subdividedCenterSurface;
	AABB subdividedSurface;

	//Position [0]
	subdividedCenterSurface = { centerSurface.x - subdividedSurfaceSize.x / 2, centerSurface.y, centerSurface.z + subdividedSurfaceSize.z / 2 };
	subdividedSurface.SetFromCenterAndSize(subdividedCenterSurface, subdividedSurfaceSize);
	childNodes.push_back(QTNode(subdividedSurface));

	//Position [1]
	subdividedCenterSurface = { centerSurface.x + subdividedSurfaceSize.x / 2, centerSurface.y, centerSurface.z + subdividedSurfaceSize.z / 2 };
	subdividedSurface.SetFromCenterAndSize(subdividedCenterSurface, subdividedSurfaceSize);
	childNodes.push_back(QTNode(subdividedSurface));

	//Position [2]
	subdividedCenterSurface = { centerSurface.x - subdividedSurfaceSize.x / 2, centerSurface.y, centerSurface.z - subdividedSurfaceSize.z / 2 };
	subdividedSurface.SetFromCenterAndSize(subdividedCenterSurface, subdividedSurfaceSize);
	childNodes.push_back(QTNode(subdividedSurface));

	//Position [3]
	subdividedCenterSurface = { centerSurface.x + subdividedSurfaceSize.x / 2, centerSurface.y, centerSurface.z - subdividedSurfaceSize.z / 2 };
	subdividedSurface.SetFromCenterAndSize(subdividedCenterSurface, subdividedSurfaceSize);
	childNodes.push_back(QTNode(subdividedSurface));
}

void QTNode::Restructure()
{
	std::vector<GameObject*>::const_iterator it = GObjectsInNode.begin();
	while ( it != GObjectsInNode.end())
	{
		unsigned int intersectionsFound = 0;
		bool intersecting[4];

		for (unsigned int i = 0; i < 4; ++i)
		{
			if (intersecting[i] = childNodes[i].GetSurface().Intersects((*it)->GetAABB()))
			{
				++intersectionsFound;
			}
		}

		if (intersectionsFound != 4)
		{
			
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (intersecting[i])
				{
					/*if (childNodes[i].GObjectsInNode.size() < maxGObjectsInNode)
					{*/
						childNodes[i].AddGameObject(*it);
					//}
				}
			}
			
			it = GObjectsInNode.erase(it);

		}
		else
		{
			++it;
		}
	}
}

void QTNode::Draw()
{
	AABB surfaceToDraw = surface;
	
	App->renderer->AddAABBQuadtree(surfaceToDraw);

	for (unsigned int i = 0; i < childNodes.size(); ++i)
	{
		if (!childNodes[i].GObjectsInNode.empty() || !childNodes[i].childNodes.empty())
		{
			childNodes[i].Draw();
		}
		
	}
}

void QTNode::Clear()
{
	GObjectsInNode.clear();
	for (QTNode child : childNodes)
	{
		child.Clear();
	}
	childNodes.clear();
}




