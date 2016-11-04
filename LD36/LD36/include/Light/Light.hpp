#pragma once
#include "Ray\Ray.hpp"
#include "World\World.hpp"

/*
	Todo:
		- Light separated into own class which will hold the numerous visibility polygons
		for each light
*/

class Light
{

public:

	explicit Light(Vector2f Position, World* pWorld);

	~Light();

	void SwitchOn() { IsLightOn_ = true; }

	void SwitchOff() { IsLightOn_ = false; }

	bool IsOn() const { return IsLightOn_; }

	Vector2f GetLightPosition() const { Position_; }

	void Draw(RenderTarget& RTarget) const;

private:

	void CreateVisibilityPolygon(Vector2f Origin);

	void CalculateUniqueAngles(Vector2f Origin);

	std::vector<VertexArray> VisibilityPolygons_;
	std::vector<float> UniqueAngles_;

	World* World_ = nullptr;

	Vector2f Position_ = Vector2f(0.f,0.f);

	bool IsLightOn_ = true;

	RenderStates RStates_;
	Shader* AttenuationShader_ = nullptr;
};
