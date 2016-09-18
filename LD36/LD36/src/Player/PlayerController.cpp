#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(ProjectileManager* ProjectileMgr, World* ActiveWorld, Vector2f ScreenSize)
	: ProjectileMgr_(ProjectileMgr), ScreenSize_(ScreenSize), World_(ActiveWorld)
{
#if IN_DEVELOPMENT_BUILD
	assert(ProjectileMgr);
	assert(ActiveWorld);
#endif
}


PlayerController::~PlayerController()
{
	for (auto B : Boats_)
	{
		B = nullptr;
	}
}

void PlayerController::AddPlayerBoats(Boat* B)
{
	//Todo: Implemenet other boats when ready
	Boats_.push_back(B);
	B->SetObjectType(ObjectType::ePlayerBoat);

	//spawn the first boat in the centre of the screen
	Boats_[Raft]->setPosition((ScreenSize_ * 0.5f) - (Boats_[Raft]->GetSize() * 0.5f));
	Boats_[Raft]->SetControlState(Player); //set the boats control staete as the boat
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
	if (B.getPosition().x <= 0.f || B.getPosition().x + B.GetSize().x > ScreenSize_.x)
	{
		B.move(MoveVec_ * Delta * -1.5f); //repel it from the wall it's crossing/crossed
	}

	CollisionData CollData = World_->CheckCollision(B);
	if (CollData.bDidCollide == true)
	{
		printf("\nPlayer Collision");
	}
}
