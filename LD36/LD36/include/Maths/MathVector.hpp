#pragma once

#include <SFML\System\Vector2.hpp>
#include <ostream>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>

/**
- SFML Vectors functionality extensions
**/


using namespace sf;
///Extension functions for SFML vectos 

///Function to normalise vector
///Param: ref Vector2
///Return: Vector2
template<typename T>
inline Vector2<T> Normalise(const Vector2<T>& Vector);

///Get the dot product between two vectors
///Param: ref Vector2
///Return: type of vector(float, int etc..)
template<typename T>
inline T DotProduct(const Vector2<T>& Vector1, const Vector2<T>& Vector2);

///Get the length of a vector
///Param: ref Vector2
///Return: type of vector 
template<typename T>
inline T GetLength(const Vector2<T>& Vector);

///Get the squared length of a vector 
///Param: ref Vector2
///Return: Type of vector
template<typename T>
inline T GetSquareLength(const Vector2<T>& Vector);

///Set a vector to 0, 0
///Param: ref Vector2
///Return: Vector2
template<typename T>
inline void Zero(Vector2<T>& Vector);

///Get angle between two vectors
///Param: ref Vector2
///Return: Type of vector
template<typename T>
inline T GetAngleBetween(const Vector2<T>& Vector);

///Rotate a vector by angle theta (in radians)
///Param: ref Vector2
///Param: float Angle
///Return: Type of vector
template<typename T>
inline Vector2<T> RotateVector(const sf::Vector2<T>& Vector, float Angle);

///Output stream operator for Vector2
template<typename T>
inline std::ostream& operator<< (std::ostream& OStream, const Vector2<T>& Vector);

///Input stream operator for Vector2
template<typename T>
inline std::istream& operator >> (std::istream& IStream, Vector2<T>& Vector);

typedef Vector2<double> Vector2d;

#include "MathVector.inl"

