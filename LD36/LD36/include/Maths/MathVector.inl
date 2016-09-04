#include "MathVector.hpp"

/////////////////////////////////////

template<typename T>
inline Vector2<T> Normalise(const Vector2<T>& Vector)
{
	T Length = static_cast<T> (GetLength(Vector));

	if (Length != 0)
	{
		return(Vector * 1.f / Length);
	}
	return Vector;

}

template<typename T>
inline T DotProduct(const Vector2<T>& Vector1, const Vector2<T>& Vector2)
{
	return{ static_cast<T> ((Vector1.x * Vector2.x) + (Vector1.y * Vector2.y)) };
}

template<typename T>
inline T GetLength(const Vector2<T>& Vector)
{
	return{ static_cast<T> (sqrt(GetSquareLength(Vector))) };
}

template<typename T>
inline T GetSquareLength(const Vector2<T>& Vector)
{
	return{ (Vector.x * Vector.x) + (Vector.y * Vector.y) };
}

template<typename T>
inline void Zero(Vector2<T>& Vector)
{
	Vector.x = 0;
	Vector.y = 0;
}

template<typename T>
inline T GetAngleBetween(const Vector2<T>& Vector)
{
	//TODO Clean up the logic of this function 
	T DP{ DotProduct(Vector) };

	if (round(GetSquareLength()) == 1)
	{
		return T{ static_cast<T> (acos(DP)) };
	}
	return{ static_cast<T> (acos(DP / (GetLength() * Vector.GetLength()))) };
}

template<typename T>
inline sf::Vector2<T> RotateVector(const Vector2<T>& Vector, float Angle)
{
	sf::Vector2<T> Vec;
	Angle = Radians(Angle);

	Vec.x = static_cast<T> ((Vector.x * cos(Angle)) - (Vector.y * sin(Angle)));
	Vec.y = static_cast<T> ((Vector.x * sin(Angle)) + (Vector.y * cos(Angle)));
	return{ Vec };
}

template<typename T>
inline std::ostream& operator<<(std::ostream & OStream, const Vector2<T>& Vector)
{
	OStream << Vector.x << " : " << Vector.y;
	return(OStream);
}

template<typename T>
inline std::istream& operator >> (std::istream& IStream, Vector2<T>& Vector)
{
	IStream >> Vector.x >> Vector.y;
	return (IStream);
}


/////////////////////////////////////