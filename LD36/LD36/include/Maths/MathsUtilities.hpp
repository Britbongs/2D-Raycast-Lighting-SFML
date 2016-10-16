#pragma once

#include <cstdlib>	
#include <ctime>

const float PI{ 3.141592f };

const float PI_OVER_2{ PI / 2.f };

const float PI_BY_2 { PI  * 2.f };

template<typename T>
T Degrees(T Theta);

template<typename T>
T Radians(T Theta);

template<typename T>
T Clamp(T Value, T Min, T Max);

template <typename T>
T Max(T A, T B);

template <typename T>
T Min(T Value, T Min);

static __forceinline void InitRand()
{
	srand(static_cast<unsigned>(time(0)));
}

//Random float between 0 - 1
static __forceinline float RandFloat()
{
	return (static_cast<float> (rand()) / static_cast<float>(RAND_MAX));
}

//Random float between 0 and Max
static __forceinline float RandFloat(float Max)
{
	if (Max != 0)
	{
		return static_cast<float>(rand()) / static_cast<float> (RAND_MAX / Max);
	}
	return(0.f);
}

//Random float between max & min
static __forceinline float RandFloat(float Min, float Max)
{
	if (Max - Min != 0)
	{
		return Min + static_cast<float> (rand()) / static_cast<float> (RAND_MAX / (Max - Min));
	}
	return{ 0.f };
}

//Lerp
static __forceinline float Lerp(float A, float B, float t) {
	return (1 - t)*A + t*B;
}


#include "MathsUtilities.inl"
