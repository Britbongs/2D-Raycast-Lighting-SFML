#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(ProjectileManager* ProjectileMgr)
	: ProjectileMgr_(ProjectileMgr)
{

	BoatTextures_[BoatType::Raft] = AssetMgr_->LoadTexture("res//textures//raft.png");

	BoatTextures_[BoatType::RowingBoat] = AssetMgr_->LoadTexture("res//textures//rowboat.png");

	BoatTextures_[BoatType::SailBoat] = AssetMgr_->LoadTexture("res//textures//sailboat.png");

	BoatTextures_[BoatType::SteamBoat] = AssetMgr_->LoadTexture("res//textures//steam_warship.png");

	BoatTextures_[BoatType::Warship] = AssetMgr_->LoadTexture("res//textures//modern_warship.png");
}


PlayerController::~PlayerController()
{

}

void PlayerController::AddPlayerBoats(Boat* Boats[BOAT_TYPE_COUNT])
{
	assert(Boats[Raft] && Boats[RowingBoat] && Boats[SailBoat] && Boats[SteamBoat] && Boats[Warship]);

	Boats_.push_back(Boats[Raft]);
	Boats_.push_back(Boats[RowingBoat]);
	Boats_.push_back(Boats[SailBoat]);
	Boats_.push_back(Boats[SteamBoat]);
	Boats_.push_back(Boats[Warship]);

}

void PlayerController::Fire(Vector2f MousePos)
{

}

void PlayerController::Move(Int32 Direc)
{

}

void PlayerController::UpdatePlayer(float Delta)
{

}
