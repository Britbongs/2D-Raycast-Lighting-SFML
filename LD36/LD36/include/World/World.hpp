#pragma once

#include "GameObject\GameObject.h"
#include "MeshCollider\MeshCollider.h"

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

	struct Projection
	{
		float Max = 0.f;
		float Min = 0.f;
	};

	//TODO Implement sat collision here
	void MeshCollisionCheck(const GameObject& SATColider);

	Projection GetProjection(const MeshCollider& Collider, const Vector2f& EdgeNormal) const;

	void AABBCollisionCheck(const GameObject& Object);

	std::vector<GameObject*> CollidersToCheck_;

	std::vector<GameObject*>& Objects_;

	RenderTexture* RTexture_;
};

