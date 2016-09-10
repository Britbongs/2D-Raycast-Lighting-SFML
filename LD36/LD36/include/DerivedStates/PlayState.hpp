#pragma once

#include "States\State.hpp"
#include "Boat\Boat.hpp"
#include "Asset\AssetManager.hpp"
#include "TiledBackground\TiledBackground.hpp"
#include "Projectile\ProjectileManager.hpp"	
#include "Player\PlayerController.hpp"
#include "AI\AIBoatManager.hpp"

class PlayState : public State

{
public:

	PlayState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture);

	~PlayState();

	virtual bool Initialise() override;

	virtual void Update(float Delta) override;

	virtual void Render() const override;

	virtual void PostRender() const override;

	virtual void HandleEvents(sf::Event& Evnt, float Delta) override;

private:

	struct PlayerData
	{
		Int32 Lives = 10;
		Int32 Ammo = 10;
		Int32 KillCount = 0;
		Int32 Type = (Int32)BoatType::Raft;
		float TimeAlive = 0.f;
		float FireTimer = 0.f;
	};


	void SetAIBoatPosition(Boat& B);

	void SpawnObstacles();

	void Respawn();

	bool PlayerCrashed();

	void CentreViewOnPlayer();

	void EvolvePlayerAndAI(BoatType Type);

	const sf::Int32 ObstacleCount_ = 200;
	const sf::Int32 BoatCount_ = 30;

	sf::RectangleShape SeaBackground_;

	std::vector<sf::RectangleShape> Obstacles_;
	std::vector<Boat*> Boats_;
	std::vector<Boat*> AIBoats_;

	sf::Text Instructions_;
	sf::Text RespawnText_;
	sf::Text LivesText_;

	sf::Sprite FinishedSplash_;

	sf::Int32 ActiveBoat_ = -1;
	sf::Int32 MaxKills_ = 2;//10; < ---- DEBUG, CHANGE ON RELEASE

	bool GameStarted_ = false;
	bool Respawning_ = false;
	bool ShowFinished_ = false; //Change 

	float MoveSpeed = 120.f;

	PlayerData PlayerData_;

	AssetManager* AssetMgr_ = nullptr;
	ProjectileManager* ProjectileMgr_ = nullptr;
	AIBoatManager* AIBoatMgr_ = nullptr;
	PlayerController* Player_ = nullptr;
	TiledBackground* BG_ = nullptr;

};
