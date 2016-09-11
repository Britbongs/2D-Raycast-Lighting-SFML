#include "stdafx.h"
#include "World\World.h"


World::World(std::vector<GameObject*>& Objects, RenderTexture* RTexture)
	: Objects_(Objects), RTexture_(RTexture)
{
}

World::~World()
{
}

CollidedWith World::CheckCollision(const FloatRect& Collider) const
{
	return NoOwner;
}

bool World::IsCollisionPresent(const ColliderData &) const
{
	std::vector<ColliderData> PossibleCollidables;

	return false;
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