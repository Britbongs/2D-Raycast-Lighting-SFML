#include "stdafx.h"
#include "World\World.h"


World::World()
{
}


World::~World()
{
}

ColliderOwner World::CheckCollision(const ColliderData & Collider) const
{
	return ColliderOwner();
}

void World::SetVectors(std::vector<Boat*>* Boats, std::vector<Obstacle*>* Obstacles)
{
	Boats_ = Boats; 
	Obstacles_ = Obstacles;
}

bool World::IsCollisionPresent(const ColliderData &) const
{
	std::vector<ColliderData> PossibleCollidables;

	return false;
}
