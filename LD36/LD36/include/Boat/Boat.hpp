#pragma once

#include "Projectile\ProjectileManager.hpp"	
#include "GameObject\GameObject.h"
#include "GameVars.hpp"

enum BoatControlState
{
	None,
	Player,
	AI
};

enum BoatType
{
	Raft,
	Canoe,
	SailBoat,
	SteamBoat,
	Warship
};


//Forward declare
struct Projectile;

class Boat : public GameObject
{

public:

	Boat(BoatControlState ControlState = BoatControlState::None, BoatType Type = BoatType::Raft);

	~Boat();

	void SetControlState(BoatControlState State);

	void SetBoatType(BoatType Type);

	BoatType GetBoatType() const { return BoatType_; }

	BoatControlState GetControlState() const { return ControlState_; }

private:

	void SetBoatTexture();

	BoatControlState ControlState_ = BoatControlState::None;

	BoatType BoatType_ = BoatType::Raft;

	sf::RectangleShape Shape_;

	sf::Texture* BoatTex_ = nullptr;
	sf::Texture* BuoyTex_ = nullptr;

	ProjectileManager* Projectile_ = nullptr;

};

