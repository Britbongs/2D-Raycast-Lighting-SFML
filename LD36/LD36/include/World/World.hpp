#pragma once

#include "GameObject\GameObject.h"
#include "MeshCollider\MeshCollider.h"
#include "TiledMap\TiledMap.hpp"
#include "GameVars.hpp"

struct WorldIntersectionData
{
	bool bDidIntersect = false;
	Vector2f CollisionResponse;
};

//SAT collider for tile 
struct TileCollisionData
{
	TileCollisionData()
		: bIsBlockedTile(false)
	{
		//Default CTOR
	}

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

	WorldIntersectionData CheckWorldIntersection(GameObject& Object, Vector2f& MovementVector);

	bool IsInsideView(const FloatRect& AABB) const;

	std::vector<TileCollisionData>* GetTileMeshColliders() { return &TileMeshColliders_; }

	std::vector<TileCollisionData>& GetTileMeshCollidersBlocked() { return TileMeshCollidersBlocked_; }
	
	std::vector<Vector2f>& GetUniqueTiledMapPoints() { return UniqueTiledMapPoints_; }

private:

	using AABB = FloatRect;

	using ColliderPair = std::pair<FloatRect, MeshCollider>;

	struct Projection
	{// Line Projection for Separating Axis Theorem 
		float Max = 0.f; //maximum value
		float Min = 0.f; //minimum value
	};

	bool DoMeshCollidersIntersect(const MeshCollider& MeshA, const MeshCollider& MeshB) const;

	Projection GetProjection(const MeshCollider& Collider, const Vector2f& EdgeNormal) const;

	bool AABBCollisionCheck(const sf::FloatRect& RectA, const sf::FloatRect& RectB) const;
	
	void CalculateUniqueTilemapPoints();

	std::vector<const MeshCollider*> AABBIntersectedTileColliders_; // List of mesh colliders from tiles which the player may be intersecting with (via AABB test) 

	std::vector<TileCollisionData> TileMeshColliders_;
	std::vector<TileCollisionData> TileMeshCollidersBlocked_;

	std::vector<GameObject*>& Objects_;

	std::vector<Vector2f> UniqueTiledMapPoints_;

	RenderTexture* RTexture_;

	Vector2i MapDimension_;

	enum TileSearchDirections : Uint32
	{
		eTile_Up_Left,
		eTile_Up,
		eTile_Up_Right,
		eTile_Right,
		eTile_Down_Right,
		eTile_Down,
		eTile_Down_Left,
		eTile_Left,
		eTile_Search_Max
	};

	FloatRect GetSearchTileAABB(TileSearchDirections Direction, const Vector2i& GridLoc) const;

	bool IsSearchTileBlocked(TileSearchDirections Direction, const Vector2i& GridLoc) const;

	Int32 GetSearchTileIndex(TileSearchDirections Direction, Vector2i GridLoc) const;
};
