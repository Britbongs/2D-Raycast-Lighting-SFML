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

class World
{
public:

	explicit World(std::vector<GameObject*>& Objects_, RenderTexture* RTexture);

	~World();

	void SetupTileMeshColliders(const TiledMap* tiledMap_);

	WorldIntersectionData CheckWorldIntersection(GameObject& Object, Vector2f& MovementVector);

	bool IsInsideView(const FloatRect& AABB) const;

private:

	using AABB = FloatRect;

	struct TileCollisionData
	{
		//Mesh collider of the tile (can be queried for collision normals & for ray cast line segment data)
		TileCollisionData(MeshCollider MeshColl, bool IsBlocked)
			: MCollider(MeshColl), bIsBlockedTile(IsBlocked)
		{}
		MeshCollider MCollider;
		bool bIsBlockedTile = false;
	};


	struct Projection
	{// Line Projection for Separating Axis Theorem 
		float Max = 0.f; //maximum value
		float Min = 0.f; //minimum value
	};

	bool DoMeshCollidersIntersect(const MeshCollider& MeshA, const MeshCollider& MeshB) const;

	Projection GetProjection(const MeshCollider& Collider, const Vector2f& EdgeNormal) const;

	bool AABBCollisionCheck(const sf::FloatRect& RectA, const sf::FloatRect& RectB) const;


	std::vector<GameObject*> CollidersToCheck_;

	std::vector<TileCollisionData> TileMeshColliders_;

	std::vector<GameObject*>& Objects_;

	RenderTexture* RTexture_;

	Vector2i MapDimension_;

	enum TileSearchDirections
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
};
