#pragma once

#include "GameObject\GameObject.h"
#include "MeshCollider\MeshCollider.h"
#include "TiledMap\TiledMap.hpp"

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

//SAT collider for tile 
struct TileCollisionData
{
	TileCollisionData(MeshCollider MeshColl, bool IsBlocked)
		: MCollider(MeshColl), bIsBlockedTile(IsBlocked)
	{}
	MeshCollider MCollider;
	bool bIsBlockedTile;
};

class World
{
public:

	explicit World(std::vector<GameObject*>& Objects_, RenderTexture* RTexture);

	~World();

	void SetupTileMeshColliders(const TiledMap* tiledMap_);

	CollisionData CheckCollision(const GameObject& Collider);

	bool IsInsideView(const FloatRect& AABB) const;

	std::vector<TileCollisionData>* GetTileMeshColliders() { return &TileMeshColliders_; }

	std::vector<TileCollisionData>& GetTileMeshCollidersBlocked() { return TileMeshCollidersBlocked_; }
	
	std::vector<Vector2f>& GetUniqueTiledMapPoints() { return UniqueTiledMapPoints_; }

private:

	using AABB = FloatRect;

	struct Projection
	{
		float Max = 0.f;
		float Min = 0.f;
	};

	void MeshCollisionCheck(const GameObject& SATColider);

	Projection GetProjection(const MeshCollider& Collider, const Vector2f& EdgeNormal) const;

	void AABBCollisionCheck(const GameObject& Object);
	
	void CalculateUniqueTilemapPoints();

	std::vector<GameObject*> CollidersToCheck_;

	std::vector<TileCollisionData> TileMeshColliders_;
	std::vector<TileCollisionData> TileMeshCollidersBlocked_;

	std::vector<GameObject*>& Objects_;

	std::vector<Vector2f> UniqueTiledMapPoints_;

	RenderTexture* RTexture_;
};
