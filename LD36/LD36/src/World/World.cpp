#include "stdafx.h"
#include "World\World.h"


World::World(std::vector<GameObject*>& Objects, RenderTexture* RTexture)
	: Objects_(Objects), RTexture_(RTexture)
{
}

World::~World()
{
}

CollisionData World::CheckCollision(const GameObject& Object)
{
	CollisionData Data;
	Data.bDidCollide = false;
	Data.ObjType = ObjectType::eEmpty;

	AABBCollisionCheck(Object);

	if (CollidersToCheck_.size() != 0)
	{
		MeshCollisionCheck(Object);

		if (CollidersToCheck_.size() != 0)
		{
			Data.bDidCollide = true;
			Data.ObjType = CollidersToCheck_[0]->GetObjectType();
		}
	}
	return Data;
}

void World::MeshCollisionCheck(const GameObject&)
{
	//std::vector<GameObject*> PossibleCollidables;
	return;
}

void World::AABBCollisionCheck(const GameObject& Object)
{
	CollidersToCheck_.clear();

	for (auto o : Objects_)
	{

		if (o == &Object)
		{//Same object, continue 
			continue;
		}


		if (!o->GetAABB().intersects(Object.GetAABB()))
		{//No intersection, continue 
			continue;
		}

		//add it to the list
		CollidersToCheck_.push_back(o);
	}
}

bool World::IsInsideView(const FloatRect& AABB) const
{
	View V = RTexture_->getView();
	FloatRect Collider;

	Collider.left = V.getCenter().x - V.getSize().x / 2.f;
	Collider.top = V.getCenter().y - V.getSize().y / 2.f;
	Collider.width = V.getSize().x;
	Collider.height = V.getSize().y;

	return Collider.contains(AABB.left, AABB.top);
}