#pragma once
// TODO rename file
struct Ray {
	
	Vector2f Origin_;
	
	Vector2f Direction_;

	Ray(Vector2f o, Vector2f d)
		: Origin_(o), Direction_(d)
	{
	}

	Vector2f GetDirection() { return Direction_; }

	Vector2f GetOrigin() { return Origin_; }

	void SetOrigin(const Vector2f& o) { Origin_ = o; }

	void SetDirection(const Vector2f& d) { Direction_ = d; }
	
	Vector3f NO_INLINE GetIntersection(const Vector2f& PointA, const Vector2f& PointB)const;

};

class Raycast
{
public:

	Raycast();
	~Raycast();


private:

};

