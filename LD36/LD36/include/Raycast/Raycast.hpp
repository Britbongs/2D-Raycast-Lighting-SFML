#pragma once

struct Ray {
	Vector2f Origin_;
	Vector2f Direction_;

	Ray(Vector2f o, Vector2f d)
		: Origin_(o), Direction_(d)
	{
	}

	Vector2f GetDirection() { return Direction_;  }

	Vector2f GetOrigin(){ return Origin_; }

	void SetOrigin(const Vector2f& o){ Origin_ = o; }

	void SetDirection(const Vector2f& d){ Direction_ = d; }
};

class Raycast
{
public:

	Raycast();
	~Raycast();	

	Vector3f GetIntersection(Ray ray, Vector2f a, Vector2f b);

private:

};

