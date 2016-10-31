#include "MathsUtilities.hpp"

/////////////////////////////////////

template<typename T>
inline T Degrees(T theta)
{
	return{ static_cast <T> (static_cast<double>(theta) * 180.0 / std::PI) };
}
template<typename T>
inline T Radians(T theta)
{
	return{ static_cast <T> (static_cast<double>(theta) * 180.0 / PI) };
}

template<typename T>
T Clamp(T Value, T Min, T Max)
{
	if (Value < Min)
		return Min;

	if (Value > Max)
		return Max;

	return (Value);
}

template<typename T>
T Max(T A, T B)
{
	return{ A > B ? A : B };
}

template <typename T>
T Min(T A, T B)
{
	return{ A < B ? A : B };
}
/////////////////////////////////////
