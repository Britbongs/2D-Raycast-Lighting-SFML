#pragma once
#include "Light\Light.hpp"

class LightMap
{

public:

	explicit LightMap(Vector2u Size, World* pWorld);
	
	~LightMap();

	void AddLight(Vector2f Origin);
	
	void ClearAllLights();

	void Draw();

	const Texture& GetTexture() const { return LightMapBuffer_.getTexture(); }

private:

	std::vector<Light> Lights_;

	RenderTexture LightMapBuffer_;

	World* World_ = nullptr;
};

