#pragma once

#include "States\State.hpp"
#include "Asset\AssetManager.hpp"
#include "TiledBackground\TiledBackground.hpp"
#include "Projectile\ProjectileManager.hpp"	
#include "Player\PlayerController.hpp"
#include "World\World.h"

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
	//TiledBackground* BG_ = nullptr;
	World* World_ = nullptr;

};
