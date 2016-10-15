#include "stdafx.hpp"
#include "Raycast/Raycast.hpp"


Raycast::Raycast()
{
}

Raycast::~Raycast()
{
}

Vector3f Raycast::GetIntersection(Ray ray, Vector2f a, Vector2f b)
{
	// RAY in parametric: Point + Direction*T1
	float r_px = ray.Origin_.x;
	float r_py = ray.Origin_.y;
	float r_dx = ray.Direction_.x - ray.Origin_.x;
	float r_dy = ray.Direction_.y - ray.Origin_.x;

	// SEGMENT in parametric: Point + Direction*T2
	float s_px = a.x;
	float s_py = a.y;
	float s_dx = b.x - a.x;
	float s_dy = b.y - a.y;

	// Are they parallel? If so, no intersect
	float r_mag = sqrt(r_dx * r_dx + r_dy * r_dy);
	float s_mag = sqrt(s_dx * s_dx + s_dy * s_dy);

	if ((r_dx / r_mag == s_dx / s_mag )&& (r_dy / r_mag == s_dy / s_mag)) //Directions are the same
	{
		return Vector3f(-1,-1,-1);
	}

	float T2 = (r_dx*(s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
	float T1 = (s_px + s_dx * T2 - r_px) / r_dx;

	// Must be within parametic whatevers for RAY/SEGMENT
	if (T1 < 0) return Vector3f(-1, -1, -1);
	if (T2 < 0 || T2 > 1) return Vector3f(-1, -1, -1);

	return Vector3f(r_px + (r_dx * T1), r_py + (r_dy * T1), T1);
}

