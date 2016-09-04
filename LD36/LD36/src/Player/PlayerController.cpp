#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(ProjectileManager* ProjectileMgr)
	: ProjectileMgr_(ProjectileMgr)
{

}


PlayerController::~PlayerController()
{

}

void PlayerController::AddBoat(Boat * B)
{
	assert(B);
	Boats_.push_back(B);
	if (B->GetBoatType() == CurrentBoat_)
	{
		Int32 Index = (std::find(Boats_.begin(), Boats_.end(), B) - Boats_.begin());
		if (Index < (signed)Boats_.size())
		{
			ActiveIndex_ = Index;
		}
	}
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
