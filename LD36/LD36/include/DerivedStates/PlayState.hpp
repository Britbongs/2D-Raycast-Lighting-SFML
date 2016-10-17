#pragma once

#include "States\State.hpp"
#include "Asset\AssetManager.hpp"
#include "TiledMap\TiledMap.hpp"
#include "Projectile\ProjectileManager.hpp"	
#include "Player\PlayerController.hpp"
#include "Raycast\Raycast.hpp"
#include "World\World.hpp"
#include "TMXLoader\TMXLoader.hpp"

class PlayState : public State

{
public:

	PlayState(sf::Int32 ID, RenderWindow* RWindow, RenderTexture* RTexture);

	~PlayState();

	virtual bool Initialise() override;

	virtual void Deinitialise() override;

	virtual void Update(float Delta) override;

	virtual void Render() override;

	virtual void PostRender() const override;

	virtual void HandleEvents(sf::Event& Evnt, float Delta) override;

private:
	
	void CalculateUniquePoints();
	std::vector<float> CalculateUniqueAngles(const Vector2f origin);
	void DrawVisibilityPolygon(const Vector2f& origin);
	void LoadShaders();
	struct PlayerData
	{	};

	const Int32 RayCount = 20;
	
	std::vector<GameObject*> GameObjects_;

	sf::CircleShape Circle_;
	Raycast RayTest_;
	std::vector<Vector2f> UniquePoints_;
	std::vector<VertexArray> VisibilityPolygons_;
	std::vector<RenderStates> LightShaders_;

	AssetManager* AssetMgr_ = nullptr;
	ProjectileManager* ProjectileMgr_ = nullptr;
	PlayerController* Player_ = nullptr;
	TiledMap* TiledMap_ = nullptr;
	World* World_ = nullptr;
	
	RenderTexture SceneRenderer_;
	RenderTexture LightMap_;
	TMXLoader Loader_;
	// TODO Move Shaders to shader manager 
	Shader AmbientShader_;
	Shader AttenuationShader_;
};
