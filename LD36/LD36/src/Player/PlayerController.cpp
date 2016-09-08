#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(ProjectileManager* ProjectileMgr)
	: ProjectileMgr_(ProjectileMgr)
{

	BoatTextures_[BoatType::Raft] = AssetMgr_->LoadTexture("res//textures//raft.png");
	assert(BoatTextures_[BoatType::Raft]);

	BoatTextures_[BoatType::RowingBoat] = AssetMgr_->LoadTexture("res//textures//rowboat.png");
	assert(BoatTextures_[BoatType::RowingBoat]);

	BoatTextures_[BoatType::SailBoat] = AssetMgr_->LoadTexture("res//textures//sailboat.png");
	assert(BoatTextures_[BoatType::SailBoat]);

	BoatTextures_[BoatType::SteamBoat] = AssetMgr_->LoadTexture("res//textures//steam_warship.png");
	assert(BoatTextures_[BoatType::SteamBoat]);

	BoatTextures_[BoatType::Warship] = AssetMgr_->LoadTexture("res//textures//modern_warship.png");
	assert(BoatTextures_[BoatType::Warship]);
}


PlayerController::~PlayerController()
{

}

void PlayerController::AddPlayerBoats(Boat* Boats[BOAT_TYPE_COUNT])
{
	assert(Boats[Raft]);
	assert(Boats[RowingBoat]);
	assert(Boats[SailBoat]);
	assert(Boats[SteamBoat]);
	assert(Boats[Warship]);

	Boats_.push_back(Boats[Raft]);
	Boats_.push_back(Boats[RowingBoat]);
	Boats_.push_back(Boats[SailBoat]);
	Boats_.push_back(Boats[SteamBoat]);
	Boats_.push_back(Boats[Warship]);

}

void PlayerController::Fire()
{
	Boat& B = *Boats_[ActiveIndex_];
	ProjectileFireData FireData;

	FireData.BoatType = B.GetBoatType();
	FireData.Direction = MoveVec_ / MOVE_SPEED;
	FireData.FiredBy = Player;
	FireData.StartPos = B.getPosition();

	ProjectileMgr_->FireProjectile(FireData);
}

void PlayerController::SetDirection(PlayerMoveDirection Move)
{
	switch (Move)
	{
	case UP:
		MoveVec_.x = cosf(Radians(0.f)) * MOVE_SPEED;
		MoveVec_.y = sinf(Radians(0.f))* MOVE_SPEED;
		break;

	case UP_LEFT:
		MoveVec_.x = cosf(Radians(-45.f)) * MOVE_SPEED;
		MoveVec_.y = sinf(Radians(-45.f)) * MOVE_SPEED;
		break;

	case UP_RIGHT:
		MoveVec_.x = cosf(Radians(45.f)) * MOVE_SPEED;
		MoveVec_.x = sinf(Radians(45.f)) * MOVE_SPEED;
		break;
	}
}

void PlayerController::UpdatePlayer(float Delta)
{

	Boat& B = *Boats_[ActiveIndex_];

	B.move(MoveVec_* Delta);
}
