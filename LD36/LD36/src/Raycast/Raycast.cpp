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
	//////// RAY in parametric: Point + Direction*T1
	//float r_px = Origin_.x;
	//float r_py = Origin_.y;
	//float r_dx = Direction_.x - Origin_.x;
	//float r_dy = Direction_.y - Origin_.y;

	//// SEGMENT in parametric: Point + Direction*T2
	//float s_px = PointA.x;
	//float s_py = PointA.y;
	//float s_dx = PointB.x - PointA.x;
	//float s_dy = PointB.y - PointA.y;

	//// Are they parallel? If so, no intersect
	//float r_mag = sqrt(r_dx * r_dx + r_dy * r_dy);
	//float s_mag = sqrt(s_dx * s_dx + s_dy * s_dy);

	//if ((r_dx / r_mag == s_dx / s_mag )&& (r_dy / r_mag == s_dy / s_mag)) //Directions are the same
	//{
	//	return Vector3f(-1,-1,-1);
	//}

	//r_dx = r_dx / r_mag;
	//r_dy = r_dy / r_mag;

	//float T2 = (r_dx*(s_py - r_py) + r_dy*(r_px - s_px)) / (s_dx*r_dy - s_dy*r_dx);
	//float T1 = (s_px + s_dx*T2 - r_px) / r_dx;


	//// Must be within parametic whatevers for RAY/SEGMENT
	//if (T1 < 0) return Vector3f(-1, -1, -1);
	//if (T2 < 0 || T2 > 1) return Vector3f(-1, -1, -1);

	//return Vector3f(r_px + (r_dx * T1), r_py + (r_dy * T1), T1);

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

