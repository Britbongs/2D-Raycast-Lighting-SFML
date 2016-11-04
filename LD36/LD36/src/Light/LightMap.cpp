#include "stdafx.hpp"
#include "Light\LightMap.hpp"
#include <future>

LightMap::LightMap(Vector2u Size, World* pWorld)
	: World_(pWorld)
{
	CHECK(pWorld);
	LightMapBuffer_.create(Size.x, Size.y);
	LightMapBuffer_.setSmooth(true);

}

LightMap::~LightMap()
{
	ClearAllLights();
}

void LightMap::AddLight(Vector2f Origin)
{
	Lights_.push_back(Light(Origin, World_));
}

void LightMap::ClearAllLights()
{
	Lights_.clear();
}

void LightMap::Draw()
{
	LightMapBuffer_.clear();
	for (auto& L : Lights_)
	{
		L.Draw(LightMapBuffer_);

	}
	LightMapBuffer_.display();
}
