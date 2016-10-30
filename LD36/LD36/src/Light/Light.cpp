#include "stdafx.hpp"
#include "Light\Light.h"
#include <future>
Light::Light(Vector2f Pos, World* pWorld)
	: Position_(Pos), World_(pWorld)
{
	float Offset = 5.f;
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_);
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(Offset, 0));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(Offset, Offset));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(0.f, Offset));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(Offset, -Offset));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(-Offset, 0));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(-Offset, Offset));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(-Offset, -Offset));
	std::async(std::launch::async, &Light::CreateVisibilityPolygon, this, Position_ + Vector2f(0.f, -Offset));
}

Light::~Light()
{
}

void Light::draw(RenderTarget & RTarget, RenderStates RStates)
{
	if (!IsLightOn_)
	{
		return;
	}
	for (auto& Poly : VisibilityPolygons_)
	{
		RTarget.draw(Poly, RStates);
	}
}

void Light::CreateVisibilityPolygon(Vector2f Origin)
{
	auto& Colliders = World_->GetTileMeshCollidersBlocked();

	CalculateUniqueAngles(Origin);

	std::vector<Vector3f> Intersections;

	Vector2f A(0.f, 0.f), B(0.f, 0.f);
	Vector3f Intersection(0.f, 0.f, 0.f);

	for (Int32 k{ 0 }; k < (Int32)UniqueAngles_.size(); ++k)
	{
		Vector2f DirectionVector(cosf(UniqueAngles_[k]), sinf(UniqueAngles_[k]));
		Ray Ray{ Origin, Vector2f(Origin.x + DirectionVector.x, Origin.y + DirectionVector.y) };

		Vector3f ClosestIntersection{ -5,-5,-5 };
		for (Int32 i{ 0 }; i < (Int32)Colliders.size(); ++i)
		{
			TileCollisionData& Data = Colliders[i];
			//If the collider isn't blocked then continue the loop
			if (!Data.bIsBlockedTile)
				continue;

			//Loop through every point in the mesh collider
			for (Int32 j{ 0 }; j < Data.MCollider.GetPointCount(); ++j)
			{
				//Point a of segment
				A = Data.MCollider.GetTransformedPoint(j);
				//Point b of segment
				B = j + 1 < Data.MCollider.GetPointCount() ? Data.MCollider.GetTransformedPoint(j + 1) : Data.MCollider.GetTransformedPoint(0);

				//Create ray
				//Get the intersection point
				Intersection = Ray.GetIntersection(A, B);

				//If GetIntersection returned early then continue
				if (Intersection.z == -1)
					continue;

				if (ClosestIntersection.z == -5 || Intersection.z < ClosestIntersection.z)
				{
					ClosestIntersection = Intersection;
				}

			}
		}
		Intersections.push_back(Vector3f(ClosestIntersection.x, ClosestIntersection.y, UniqueAngles_[k]));
	}


	//Sort intersections by angle
	Color Col = Color::Cyan;
	//Col.a = 10;
	Col.a = (rand() % 255) + 10;
	DebugPrintF(DebugLog, L"Col - %i", Col.a);
	sort(Intersections.begin(), Intersections.end(), [](Vector3f const& a, Vector3f const& b) { return a.z < b.z; });


	VertexArray VisibilityPolygon = VertexArray(TrianglesFan, Intersections.size() + 2);

	for (Int32 i = 0; i < (Int32)Intersections.size(); ++i)
	{
		VisibilityPolygon[i + 1].position = Vector2f(Intersections[i].x, Intersections[i].y);
		VisibilityPolygon[i + 1].color = Col;
	}

	VisibilityPolygon[0].position = Origin;
	VisibilityPolygon[0].color = Col;
	VisibilityPolygon[VisibilityPolygon.getVertexCount() - 1].position = VisibilityPolygon[1].position;
	VisibilityPolygon[VisibilityPolygon.getVertexCount() - 1].color = Col;


	VisibilityPolygons_.push_back(VisibilityPolygon);
}

void Light::CalculateUniqueAngles(Vector2f Origin)
{
	auto& UniquePoints = World_->GetUniqueTiledMapPoints();
	UniqueAngles_.clear();

	for (int i{ 0 }; i < (Int32)UniquePoints.size(); ++i)
	{
		float angle = atan2(UniquePoints[i].y - Origin.y, UniquePoints[i].x - Origin.x);
		UniqueAngles_.push_back(angle - 0.0001f);
		UniqueAngles_.push_back(angle);
		UniqueAngles_.push_back(angle + 0.0001f);
	}
}
