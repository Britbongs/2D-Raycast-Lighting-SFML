#pragma once

class Boat;

struct Obstacle;

enum ColliderOwner
{
	None,
	AI_Boat,
	Player_Boat,
	ObstacleCollider,
	Follower
};

struct ColliderData
{
	ColliderOwner Owner = ColliderOwner::None;
	FloatRect AABB;
	//TODO - Sat collider data field added
};

class World
{
public:

	World();
	~World();
	ColliderOwner CheckCollision(const ColliderData& Collider) const;
	void SetVectors(std::vector<Boat*>* Boats, std::vector<Obstacle*>* Obstacles);
private:
	//TODO Implement sat collision here
	bool IsCollisionPresent(const ColliderData&) const;

	std::vector<Boat*>* Boats_ = nullptr;
	std::vector<Obstacle*>* Obstacles_ = nullptr;

};

