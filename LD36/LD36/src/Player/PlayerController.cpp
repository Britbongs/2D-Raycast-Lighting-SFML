#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(ProjectileManager* ProjectileMgr, Vector2f ScreenSize)
	: ProjectileMgr_(ProjectileMgr), ScreenSize_(ScreenSize)
{
	AssetMgr_ = AssetManager::GetInstance();

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

	BuoyTexture_ = AssetMgr_->LoadTexture("res//textures//buoy.png");
	assert(BuoyTexture_);
}


PlayerController::~PlayerController()
{

}

void PlayerController::AddPlayerBoats(std::vector<Boat>& Boats)
{
	Boats_.push_back(&Boats[Raft]);
	Boats_.back()->SetTexture(BoatTextures_[Raft], BuoyTexture_);
	/*Boats_.push_back(&Boats[RowingBoat]);
	Boats_.push_back(&Boats[SailBoat]);
	Boats_.push_back(&Boats[SteamBoat]);
	Boats_.push_back(&Boats[Warship]);*/

	//spawn the first boat in the centre of the screen

	Boats_[Raft]->setPosition((ScreenSize_*0.5f) - (Vector2f(Boats_[Raft]->GetGlobalBounds().width, Boats_[Raft]->GetGlobalBounds().height) * 0.5f));
	Boats_[Raft]->SetControlState(Player);
}

void PlayerController::Fire()
{
	Boat& B = *Boats_[CurrentBoat_];
	ProjectileFireData FireData;

	FireData.BoatType = B.GetBoatType();
	FireData.Direction = MoveVec_ / MOVE_SPEED;
	FireData.FiredBy = Player;
	FireData.StartPos = B.getPosition();

	ProjectileMgr_->FireProjectile(FireData);
}

void PlayerController::SetDirection(PlayerMoveDirection Move)
{
	float theta = Degrees(GetAngle(MoveVec_));

	switch (Move)
	{
	case UP:
		MoveVec_.x = 0.f;
		MoveVec_.y = -MOVE_SPEED;
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

	Boat& B = *Boats_[CurrentBoat_];

	B.move(MoveVec_* Delta);

	if (B.getPosition().x <= 0.f || B.getPosition().x + B.GetGlobalBounds().width > ScreenSize_.x)
	{
		B.move(MoveVec_ * Delta * -1.5f);
	}
}
