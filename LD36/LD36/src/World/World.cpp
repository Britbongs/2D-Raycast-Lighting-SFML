#include "stdafx.hpp"
#include "World\World.hpp"


World::World(std::vector<GameObject*>& Objects, RenderTexture* RTexture)
	: Objects_(Objects), RTexture_(RTexture)
{
}

World::~World()
{
}


void World::SetupTileMeshColliders(const TiledMap* InTileMap)
{
	Vector2i Size = InTileMap->GetGridSize();

	Int32 TileSize = InTileMap->GetTileSize();

	std::vector<Vector2f> TempVertArray;
	TempVertArray.resize(4);

	for (Int32 i{ 0 }; i < Size.x; ++i)
	{
		for (Int32 j{ 0 }; j < Size.y; ++j)
		{
			//Get index in vector
			Int32 Index = i + j * Size.x;

			//If the tile is collideable
			//Top left
			TempVertArray[0].x = (float)i * TileSize;
			TempVertArray[0].y = (float)j * TileSize;

			//Top right
			TempVertArray[1].x = (float)(i + 1) * TileSize;
			TempVertArray[1].y = (float)j * TileSize;

			//Bottom right
			TempVertArray[2].x = (float)(i + 1) * TileSize;
			TempVertArray[2].y = (float)(j + 1) * TileSize;

			//Bottom left
			TempVertArray[3].x = (float)i * TileSize;
			TempVertArray[3].y = (float)(j + 1) * TileSize;

			//Create the collider and push it into the vector
			TileCollisionData Data(MeshCollider(TempVertArray), InTileMap->GetCollideableAtIndex(Index));
			TileMeshColliders_.push_back(Data);
		}
	}
	DebugPrintF(DebugLog, L"Tiled Map Collision Data Memory Footprint: %d Bytes", sizeof(TileCollisionData) * TileMeshColliders_.size());

	for (Int32 i{ 0 }; i < Size.x; ++i)
	{
		for (Int32 j{ 0 }; j < Size.y; ++j)
		{
			Int32 Index = i + j * Size.x;

			if (!InTileMap->GetCollideableAtIndex(Index))
			{
				std::wcout << L"0";
			}
			else
			{
				std::wcout << L"1";
			}
		}
		std::wcout << endl;
	}
}

bool World::CheckCollision(const GameObject& Object)
{

	AABBCollisionCheck(Object);

	if (CollidersToCheck_.size() != 0)
	{
		MeshCollisionCheck(Object);

		if (CollidersToCheck_.size() != 0)
		{
			return true;
			//TOOD change object type in data struct to use new object types
		}
	}
	return false;
}

void World::MeshCollisionCheck(const GameObject& ObjectA)
{
	//TODO catch multiple collision resolution 
	//-----------------------------------------
	auto DoProjectionsOverlap = [=](const Projection& ProjA, const Projection& ProjB)
	{
		return ProjB.Max >= ProjA.Min && ProjB.Min <= ProjA.Max;
	};
	std::vector<GameObject*> TempColliderVector = CollidersToCheck_;
	CollidersToCheck_.clear();

	Projection ProjectionA, ProjectionB;
	for (auto ObjectB : TempColliderVector)
	{
		bool EarlyOut = false;
		Int32 i = 0;
		while (i < ObjectA.GetMeshCollider().GetNormalListSize() && !EarlyOut)
		{
			ProjectionA = GetProjection(ObjectA.GetMeshCollider(), ObjectA.GetMeshCollider().GetNormal(i));
			ProjectionB = GetProjection((*ObjectB).GetMeshCollider(), ObjectA.GetMeshCollider().GetNormal(i));
			if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
			{
				EarlyOut = true;
			}
			++i;
		}

		if (EarlyOut)
			continue;
		i = 0;

		while (i < ObjectA.GetMeshCollider().GetNormalListSize() && !EarlyOut)
		{
			ProjectionA = GetProjection(ObjectA.GetMeshCollider(), ObjectB->GetMeshCollider().GetNormal(i));
			ProjectionB = GetProjection((*ObjectB).GetMeshCollider(), ObjectB->GetMeshCollider().GetNormal(i));
			if (!DoProjectionsOverlap(ProjectionA, ProjectionB))
			{
				EarlyOut = true;
			}
			++i;
		}

		if (EarlyOut)
			continue;

		CollidersToCheck_.push_back(ObjectB);
	}

	return;
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

void World::AABBCollisionCheck(const GameObject& Object)
{
	CollidersToCheck_.clear();

	for (auto o : Objects_)
	{

		if (o == &Object)
		{//Same object, continue 
			continue;
		}


		if (!o->GetAABB().intersects(Object.GetAABB()))
		{//No intersection, continue 
			continue;
		}

		//add it to the list
		CollidersToCheck_.push_back(o);
	}
}

WorldIntersectionData World::CheckWorldIntersection(GameObject & Object, Vector2f& MovementVector)
{
	WorldIntersectionData IntersectData;
	Vector2i PlayerTilePosition = Vector2i(Object.getPosition() / (float)TILE_SIZE);

	for (Int32 i = 0; i < STATIC_CAST(Int32, TileSearchDirections::eTile_Search_Max); ++i)
	{

	}

	return IntersectData;
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