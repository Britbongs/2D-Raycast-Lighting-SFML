#pragma once

#include "States\State.hpp"
#include "Asset\AssetManager.hpp"
#include "TiledMap\TiledMap.hpp"
#include "Projectile\ProjectileManager.hpp"	
#include "Player\PlayerController.hpp"
#include "World\World.hpp"
#include "TMXLoader\TMXLoader.hpp"

class PlayState : public State

{
public:

	PlayState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture);

	~PlayState();

	virtual bool Initialise() override;

	virtual void Deinitialise() override;

	virtual void Update(float Delta) override;

	virtual void Render() const override;

	virtual void PostRender() const override;

	virtual void HandleEvents(sf::Event& Evnt, float Delta) override;

private:

	struct PlayerData
	{	};

	std::vector<GameObject*> GameObjects_;

	AssetManager* AssetMgr_ = nullptr;
	ProjectileManager* ProjectileMgr_ = nullptr;
	PlayerController* Player_ = nullptr;
	TiledMap* TiledMap_ = nullptr;
	World* World_ = nullptr;
	TMXLoader Loader_; 
};
