#include "stdafx.hpp"
#include "World\World.hpp"
#include <cfloat> //For FLT_MAX

World::World(std::vector<GameObject*>& Objects, RenderTexture* RTexture)
	: Objects_(Objects), RTexture_(RTexture)
{
}

World::~World()
{
}


void World::SetupTileMeshColliders(const TiledMap* InTileMap)
{
	MapDimension_ = InTileMap->GetGridSize();

	Int32 TileSize = InTileMap->GetTileSize();

	std::vector<Vector2f> TempVertArray;
	TempVertArray.resize(4);
	TileMeshColliders_.resize(InTileMap->GetGridSize().x * InTileMap->GetGridSize().y);
	for (Int32 i{ 0 }; i < MapDimension_.x; ++i)
	{
		for (Int32 j{ 0 }; j < MapDimension_.y; ++j)
		{
			//Get index in vector
			Int32 Index = i + j * MapDimension_.x;

			//If the tile is collideable
			//Top left
			TempVertArray[0].x = STATIC_CAST(float, i * TileSize);
			TempVertArray[0].y = STATIC_CAST(float, j * TileSize);

			//Top right
			TempVertArray[1].x = STATIC_CAST(float, (i + 1) * TileSize);
			TempVertArray[1].y = STATIC_CAST(float, j * TileSize);

			//Bottom right
			TempVertArray[2].x = STATIC_CAST(float, (i + 1) * TileSize);
			TempVertArray[2].y = STATIC_CAST(float, (j + 1) * TileSize);

			//Bottom left
			TempVertArray[3].x = STATIC_CAST(float, i * TileSize);
			TempVertArray[3].y = STATIC_CAST(float, (j + 1) * TileSize);

			//Create the collider and push it into the vector
			TileCollisionData Data(MeshCollider(TempVertArray), InTileMap->IsCollideableAtPos(Vector2i(i, j)));
			TileMeshColliders_[Index] = Data;

			if (Data.bIsBlockedTile)
			{
				TileMeshCollidersBlocked_.push_back(Data);
			}

		}
	}
	DebugPrintF(DebugLog, L"Tiled Map Collision Data Memory Footprint: %d Bytes", sizeof(TileCollisionData) * TileMeshColliders_.size());
	CalculateUniqueTilemapPoints();
}

WorldIntersectionData World::DoMeshCollidersIntersect(const MeshCollider& MeshA, const MeshCollider& MeshB) const
{
	WorldIntersectionData CollData;
	
	auto DoProjectionsOverlap = [](const Projection& ProjA, const Projection& ProjB)
	{
		return !(ProjA.Min > ProjB.Max  || ProjB.Min > ProjA.Max);
	};

	auto GetOverlapAmount = [&DoProjectionsOverlap](const Projection& ProjA, const Projection& ProjB)
	{
		if (DoProjectionsOverlap(ProjA, ProjB))
		{
			return Min(ProjA.Max, ProjA.Min) - Max(ProjA.Min, ProjB.Min);
		}
		return 0.0f;
	};

	auto DoesProjectionContain = [](const Projection& ProjA, const Projection& ProjB)
	{
		return ProjA.Min > ProjB.Min && ProjA.Max < ProjB.Max;
	};

	Projection ProjectionA, ProjectionB;
	// MTV = Minimum Translation Vector 
	Vector2f MTVAxis;  //Direction component
	float MTVOverlap = FLT_MAX; // Magnitude (initialised as large value to allow smaller values to overwrite this one) 
	
	for (Int32 i = 0; i < MeshA.GetNormalListSize(); ++i)
	{
		ProjectionA = GetProjection(MeshA, MeshA.GetNormal(i));
		ProjectionB = GetProjection(MeshB, MeshA.GetNormal(i));

		if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
		{
			CollData.bDidIntersect = false;
			return CollData;
		}
		else
		{
			float O = GetOverlapAmount(ProjectionA, ProjectionB);
			if (DoesProjectionContain(ProjectionA, ProjectionB) || DoesProjectionContain(ProjectionB, ProjectionA))
			{
				float MIN = fabs(ProjectionA.Min - ProjectionB.Min);
				float MAX = fabs(ProjectionA.Max - ProjectionB.Max);
				if (MIN < MAX)
				{
					O += MIN;
				}
				else
				{
					O += MAX;
				}
			}
			if (O < MTVOverlap)
			{
				MTVAxis = MeshA.GetNormal(i);
				MTVOverlap = O;
			}
		}
	}

	for (Int32 i = 0; i < MeshA.GetNormalListSize(); ++i)
	{
		ProjectionA = GetProjection(MeshA, MeshB.GetNormal(i));
		ProjectionB = GetProjection(MeshB, MeshB.GetNormal(i));
		if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
		{
			CollData.bDidIntersect = false;
			return CollData;
		}
		else
		{
			float O = GetOverlapAmount(ProjectionA, ProjectionB);
			if (DoesProjectionContain(ProjectionA, ProjectionB) || DoesProjectionContain(ProjectionB, ProjectionA))
			{
				float MIN = fabs(ProjectionA.Min - ProjectionB.Min);
				float MAX = fabs(ProjectionA.Max - ProjectionB.Max);
				if (MIN < MAX)
				{
					O += MIN;
				}
				else
				{
					O += MAX;
				}
			}
			if (O < MTVOverlap)
			{
				MTVAxis = MeshA.GetNormal(i);
				MTVOverlap = O;
			}
		}
	}

	MTVAxis *= MTVOverlap;
	CollData.bDidIntersect = true; 
	CollData.CollisionResponse = MTVAxis;
	
	std::cout << "MTV Axis : " <<  MTVAxis << std::endl;
	
	return CollData;
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

WorldIntersectionData World::CheckWorldIntersection(GameObject & Object, Vector2f& MovementVector)
{
	WorldIntersectionData IntersectData;
	IntersectData.bDidIntersect = false;
	IntersectData.CollisionResponse = MovementVector;
	
	Vector2i PlayerGridPos = Vector2i((Object.getPosition()) / STATIC_CAST(float, TILE_SIZE));
	Int32 i = 0;
	bool bEarlyOut = false;
	AABBIntersectedTileColliders_.clear();

	auto MoveAABB = [](const FloatRect& Rect, const Vector2f& MoveVector)
	{//Lambda to return a moved floatrect 

		return FloatRect(Vector2f(Rect.left, Rect.top) + MoveVector, Vector2f(Rect.width, Rect.height));
	};

	ColliderPair ColPair[2];
	Transform T1(Object.getTransform()), T2(Object.getTransform()); // Create 2 transform matrices with objects transform 
	ColPair[1].second = ColPair[0].second = Object.GetMeshCollider();

	ColPair[0].first = MoveAABB(Object.GetAABB(), Vector2f(MovementVector.x, 0.f));
	T1.translate(MovementVector.x, 0.f);
	ColPair[0].second.UpdatMeshCollider(T1);

	ColPair[1].first = MoveAABB(Object.GetAABB(), Vector2f(0.f, MovementVector.y));
	T2.translate(0.f, MovementVector.y);
	ColPair[1].second.UpdatMeshCollider(T2);


	//Loop through all search directions 
	//If the search tile is not a valid index, early out
	//if the players aabb moving in the movement direction is found to be colliding 
	//add the intersecting tile to a vector which will be iterated over with more precise MeshCollider
	for (Int32 ColPairIndex = 0; ColPairIndex < 2; ++ColPairIndex)
	{
		for (Int32 i = 0; i < STATIC_CAST(Int32, TileSearchDirections::eTile_Search_Max); ++i)
		{
			TileSearchDirections Direction = STATIC_CAST(TileSearchDirections, i);

			Int32 TileIndex = GetSearchTileIndex(Direction, PlayerGridPos);

			if (TileIndex == INDEX_NONE)
			{
				continue;
			}

			if (!IsSearchTileBlocked(Direction, PlayerGridPos))
			{
				continue;
			}

			//FloatRect TransformedAABB = Object.GetAABB();
			FloatRect BlockedTileAABB = GetSearchTileAABB(Direction, PlayerGridPos);
			// if search tile is classed as a blocked tile 
			//if (AABBCollisionCheck(TransformedAABB, BlockedTileAABB))
			if(AABBCollisionCheck(ColPair[ColPairIndex].first, BlockedTileAABB))
			{
				AABBIntersectedTileColliders_.push_back(&TileMeshColliders_[TileIndex].MCollider);
			}
		}
		
		bool bDidFindCollision = false;
		
		WorldIntersectionData MeshResult; 
		
		for (auto Collider : AABBIntersectedTileColliders_)
		{
			//WorldIntersectionData CollData = DoMeshCollidersIntersect(Object.GetMeshCollider(), *Collider);
			MeshResult = DoMeshCollidersIntersect(Object.GetMeshCollider(), *Collider);
			if (MeshResult.bDidIntersect) 
			{
				break;
			}
		}
		
		if (MeshResult.bDidIntersect)
		{
			//TODO remove the previous collision response if MTV works 
			if (ColPairIndex == 0)
			{// X-axis collision
				IntersectData.CollisionResponse.x = MeshResult.CollisionResponse.x; 
			}
			else
			{//Y-Axis Collision
				IntersectData.CollisionResponse.y = MeshResult.CollisionResponse.y;
			}
			IntersectData.bDidIntersect = true;
			continue;
		}
	}

	return IntersectData;
}

bool World::AABBCollisionCheck(const sf::FloatRect& RectA, const sf::FloatRect& RectB) const
{
	return RectA.intersects(RectB);
}

FloatRect World::GetSearchTileAABB(TileSearchDirections Direction, const Vector2i & GridLoc) const
{
	/*
		-Function which when passed which of the 8 tiles surrounding the player you would like the AABB for
		it will return it. When using check for an x & y value < 0 which will indicate you're out of bounds with an enum value
	*/
	FloatRect TileAABB(-1.f, -1.f, STATIC_CAST(float, TILE_SIZE), STATIC_CAST(float, TILE_SIZE));
	switch (Direction)
	{
	case eTile_Up_Left:
		TileAABB.left = STATIC_CAST(float, (GridLoc.x - 1) * TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, (GridLoc.y - 1) * TILE_SIZE);
		break;

	case eTile_Up:
		TileAABB.left = STATIC_CAST(float, GridLoc.x * TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, (GridLoc.y - 1) * TILE_SIZE);
		break;

	case eTile_Up_Right:
		TileAABB.left = STATIC_CAST(float, (GridLoc.x + 1) * TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, (GridLoc.y - 1) * TILE_SIZE);
		break;

	case eTile_Right:
		TileAABB.left = STATIC_CAST(float, (GridLoc.x + 1)* TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, GridLoc.y * TILE_SIZE);
		break;

	case eTile_Down_Right:
		TileAABB.left = STATIC_CAST(float, (GridLoc.x + 1)* TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, (GridLoc.y + 1)* TILE_SIZE);
		break;

	case eTile_Down:
		TileAABB.left = STATIC_CAST(float, GridLoc.x * TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, (GridLoc.y + 1)* TILE_SIZE);
		break;

	case eTile_Down_Left:
		TileAABB.left = STATIC_CAST(float, (GridLoc.x - 1)* TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, (GridLoc.y + 1) * TILE_SIZE);
		break;

	case eTile_Left:
		TileAABB.left = STATIC_CAST(float, (GridLoc.x - 1) * TILE_SIZE);
		TileAABB.top = STATIC_CAST(float, GridLoc.y * TILE_SIZE);
		break;

	}

	return TileAABB;
}

bool World::IsSearchTileBlocked(TileSearchDirections Direction, const Vector2i& GridLoc) const
{
	// may cause unexpected behaviour for grid locations outside the map 
	// but presently staves off a vector subscript out of range error (still in progress of fixing) 
	Int32 Index = GetSearchTileIndex(Direction, GridLoc);

	if (Index != INDEX_NONE && Index >= 0 && Index < STATIC_CAST(Int32, TileMeshColliders_.size()))
	{
		return TileMeshColliders_[Index].bIsBlockedTile;
	}

	return false;
}

Int32 World::GetSearchTileIndex(TileSearchDirections Direction, Vector2i GridLoc) const
{
	Int32 Index = 0;

	switch (Direction)
	{
	case eTile_Up_Left:
		Index = (GridLoc.x - 1) + (GridLoc.y - 1) * MapDimension_.x;
		break;

	case eTile_Up:
		Index = GridLoc.x + (GridLoc.y - 1) * MapDimension_.x;
		break;

	case eTile_Up_Right:
		Index = (GridLoc.x + 1) + (GridLoc.y - 1) * MapDimension_.x;
		break;

	case eTile_Right:
		Index = (GridLoc.x + 1) + GridLoc.y * MapDimension_.x;
		break;

	case eTile_Down_Right:
		Index = (GridLoc.x + 1) + (GridLoc.y + 1)* MapDimension_.x;
		break;

	case eTile_Down:
		Index = GridLoc.x + (GridLoc.y + 1)* MapDimension_.x;
		break;

	case eTile_Down_Left:
		Index = (GridLoc.x - 1) + (GridLoc.y + 1)* MapDimension_.x;
		break;

	case eTile_Left:
		Index = (GridLoc.x - 1) + GridLoc.y* MapDimension_.x;
		break;
	default:
		Index = INDEX_NONE;
	}

	return Index;
}

void World::CalculateUniqueTilemapPoints()
{
	std::vector<TileCollisionData> Colliders = GetTileMeshCollidersBlocked();

	std::vector<Vector2f> Points;

	for (int i{ 0 }; i < STATIC_CAST(Int32, Colliders.size()); ++i)
	{
		for (int j{ 0 }; j < Colliders[i].MCollider.GetPointCount(); ++j)
		{
			Points.push_back(Colliders[i].MCollider.GetTransformedPoint(j));
		}
	}

	for (int i{ 0 }; i < STATIC_CAST(Int32, Points.size()); ++i)
	{
		if (find(UniqueTiledMapPoints_.begin(), UniqueTiledMapPoints_.end(), Points[i]) != UniqueTiledMapPoints_.end())
		{
			continue;
		}

		UniqueTiledMapPoints_.push_back(Points[i]);
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
