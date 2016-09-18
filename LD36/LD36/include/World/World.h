#pragma once

#include "GameObject\GameObject.h"

enum ObjectType
{
	eEmpty,
	eAIBoat,
	ePlayerBoat,
	eObstacle,
	eProjectile,
	eFollower
};

//did collide & owner
using CollisionData = std::pair<bool, ObjectType>;

struct CollisionData
{
	bool bDidCollide = false; 
	ObjectType ObjType = ObjectType::eEmpty;
};

class World
{
public:

	explicit World(std::vector<GameObject*>& Objects_, RenderTexture* RTexture);

	~World();

	CollisionData CheckCollision(const GameObject& Collider);

	bool IsInsideView(const FloatRect& AABB) const;

private:
	using AABB = FloatRect;
	//TODO Implement sat collision here
	void MeshCollisionCheck(const GameObject& SATColider);

	void AABBCollisionCheck(const GameObject& Object);

	std::vector<GameObject*> CollidersToCheck_;

	std::vector<GameObject*>& Objects_;

	RenderTexture* RTexture_;
};

