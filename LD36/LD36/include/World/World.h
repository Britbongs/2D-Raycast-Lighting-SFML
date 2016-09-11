#pragma once

#include "GameObject\GameObject.h"
#include "Boat\Boat.hpp"

enum CollidedWith
{
	NoOwner,
	AI_Boat,
	Player_Boat,
	ObstacleCollider,
	Follower
};

class World
{
public:

	explicit World(std::vector<GameObject*>& Objects_, RenderTexture* RTexture);

	~World();

	CollidedWith CheckCollision(const FloatRect& Collider) const;

	bool IsInsideView(const FloatRect& AABB) const;

private:
	//TODO Implement sat collision here
	bool IsCollisionPresent(const ColliderData&) const;

	std::vector<GameObject*>Objects_;

	RenderTexture* RTexture_;
};

