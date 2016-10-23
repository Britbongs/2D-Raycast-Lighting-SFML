#include "stdafx.hpp"
#include "Raycast/Raycast.hpp"


Raycast::Raycast()
{
}

Raycast::~Raycast()
{
}

Vector3f Ray::GetIntersection(const Vector2f& PointA, const Vector2f& PointB) const
{
	Vector2f DeltaRay = Direction_ - Origin_;
	Vector2f DeltaSegment = PointB - PointA;
	float RayMagnitude = GetLength(DeltaRay);
	float SegmentMagnitude = GetLength(DeltaSegment);

	if (DeltaRay / RayMagnitude == DeltaSegment / SegmentMagnitude)
	{
		return Vector3f(-1.f, -1.f, -1.f);
	}

	DeltaRay /= RayMagnitude;

	float T2 = (DeltaRay.x *(PointA.y - Origin_.y) + DeltaRay.y * (Origin_.x - PointA.x));
	T2 /= (DeltaSegment.x * DeltaRay.y - DeltaSegment.y * DeltaRay.x);

	float T1 = (PointA.x + DeltaSegment.x  * T2 - Origin_.x) / DeltaRay.x;

	if (T1 < 0)
	{
		return Vector3f(-1.f, -1.f, -1.f);
	}

	if (T2 < 0 || T2 > 1)
	{
		return Vector3f(-1, -1, -1);
	}

	return Vector3f(Origin_.x + (DeltaRay.x * T1), Origin_.y + (DeltaRay.y * T1), T1);
}
