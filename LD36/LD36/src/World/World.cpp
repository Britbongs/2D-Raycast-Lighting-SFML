#include "stdafx.hpp"
#include "World\World.hpp"


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

void World::MeshCollisionCheck(const GameObject& ObjectA)
{
	//TODO catch multiple collision resolution 
	//-----------------------------------------
	auto DoProjectionsOverlap = [=](const Projection& ProjA, const Projection& ProjB)
	{
		return ProjB.Max >= ProjA.Min && ProjB.Min <= ProjA.Max;
	};
	std::vector<GameObject*> TempColliderVector = CollidersToCheck_;
	CollidersToCheck_.clear();

	Projection ProjectionA, ProjectionB;
	for (auto ObjectB : TempColliderVector)
	{
		bool EarlyOut = false;
		Int32 i = 0;
		while (i < ObjectA.GetMeshCollider().GetNormalListSize() && !EarlyOut)
		{
			ProjectionA = GetProjection(ObjectA.GetMeshCollider(), ObjectA.GetMeshCollider().GetNormal(i));
			ProjectionB = GetProjection((*ObjectB).GetMeshCollider(), ObjectA.GetMeshCollider().GetNormal(i));
			if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
			{
				EarlyOut = true;
			}
			++i;
		}

		if (EarlyOut)
			continue;
		i = 0;

		while (i < ObjectA.GetMeshCollider().GetNormalListSize() && !EarlyOut)
		{
			ProjectionA = GetProjection(ObjectA.GetMeshCollider(), ObjectB->GetMeshCollider().GetNormal(i));
			ProjectionB = GetProjection((*ObjectB).GetMeshCollider(), ObjectB->GetMeshCollider().GetNormal(i));
			if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
			{
				EarlyOut = true;
			}
			++i;
		}

		if (EarlyOut)
			continue;

		CollidersToCheck_.push_back(ObjectB);
	}

	return;
}

World::Projection World::GetProjection(const MeshCollider & Collider, const Vector2f & EdgeNormal) const
{
	Projection Proj;

	Proj.Max = Proj.Min = DotProduct(EdgeNormal, Collider.GetTransformedPoint(0));

	for (Int32 i = 1; i < Collider.GetPointCount(); ++i)
	{
		float DP = DotProduct(EdgeNormal, Collider.GetTransformedPoint(i));
		Proj.Max = Max(DP, Proj.Max);
		Proj.Min = Min(DP, Proj.Min);
	}

	return Proj;
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