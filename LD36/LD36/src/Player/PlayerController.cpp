#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(ProjectileManager* ProjectileMgr, Vector2f ScreenSize)
	: ProjectileMgr_(ProjectileMgr), ScreenSize_(ScreenSize)
{
	AssetMgr_ = AssetManager::GetInstance();
	// Load boat textures
	BoatTextures_[BoatType::Raft] = AssetMgr_->LoadTexture("res//textures//raft.png");
	BoatTextures_[BoatType::RowingBoat] = AssetMgr_->LoadTexture("res//textures//rowboat.png");
	BoatTextures_[BoatType::SailBoat] = AssetMgr_->LoadTexture("res//textures//sailboat.png");
	BoatTextures_[BoatType::SteamBoat] = AssetMgr_->LoadTexture("res//textures//steam_warship.png");
	BoatTextures_[BoatType::Warship] = AssetMgr_->LoadTexture("res//textures//modern_warship.png");
	
	//Load buoy texture
	BuoyTexture_ = AssetMgr_->LoadTexture("res//textures//buoy.png");
}


PlayerController::~PlayerController()
{

}

void PlayerController::AddPlayerBoats(std::vector<Boat*>& Boats)
{
	//Todo: Implemenet other boats when ready
	Boats_.push_back(Boats[Raft]);
	Boats_.back()->SetTexture(BoatTextures_[Raft], BuoyTexture_);

	//spawn the first boat in the centre of the screen
	Boats_[Raft]->setPosition((ScreenSize_ * 0.5f) - (Vector2f(Boats_[Raft]->GetGlobalBounds().width, Boats_[Raft]->GetGlobalBounds().height) * 0.5f));
	Boats_[Raft]->SetControlState(Player); //set the boats control staete as the boat

	for (auto B : Boats_)
	{
		//set the origin of each shape to the centre 
		B->setOrigin(Vector2f(B->GetSize()) * 0.5f);
	}
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
	switch (Move)
	{
	case UP:
		MoveVec_.x = 0.f;
		MoveVec_.y = -MOVE_SPEED;
		Boats_[CurrentBoat_]->setRotation(0.f);
		break;

	case UP_LEFT:
		MoveVec_.x = cosf(Radians(-45.f)) * MOVE_SPEED;
		MoveVec_.y = sinf(Radians(-45.f)) * MOVE_SPEED;
		Boats_[CurrentBoat_]->setRotation(-45.f);
		break;

	case UP_RIGHT:
		MoveVec_.x = cosf(Radians(45.f)) * MOVE_SPEED;
		MoveVec_.x = sinf(Radians(45.f)) * MOVE_SPEED;
		Boats_[CurrentBoat_]->setRotation(45.f);
		break;
	}
}

void PlayerController::UpdatePlayer(float Delta)
{
	Boat& B = *Boats_[CurrentBoat_];

	B.move(MoveVec_* Delta);

	//boat off to screen on left or right side
	if (B.getPosition().x <= 0.f || B.getPosition().x + B.GetGlobalBounds().width > ScreenSize_.x)
	{
		B.move(MoveVec_ * Delta * -1.5f); //repel it from the wall it's crossing/crossed
	}
}
