#include "stdafx.h"
#include "Boat\Boat.hpp"
#include "DerivedStates\PlayState.hpp"

using namespace sf;

Boat::Boat(BoatControlState ControlState, BoatType Type)
	: ControlState_(ControlState), BoatType_(Type)
{
	SetBoatTexture();
	BuoyTex_ = AssetManager::GetInstance()->LoadTexture("res//textures//buoy.png");
}

Boat::~Boat()
{

}

void Boat::SetControlState(BoatControlState State)
{
	ControlState_ = State;
	switch (State)
	{
	case None: SetInActive(); break;
	default: SetActive();
	}
}

void Boat::SetBoatType(BoatType Type)
{
	BoatType_ = Type;
	SetBoatTexture();
}

void Boat::SetBoatTexture()
{
	switch (BoatType_)
	{
	case Canoe:
		SetTexture(AssetManager::GetInstance()->LoadTexture("res//textures//rowboat.png"));
		break;

	case SailBoat:
		SetTexture(AssetManager::GetInstance()->LoadTexture("res//textures//sailboat.png"));
		break;

	case SteamBoat:
		SetTexture(AssetManager::GetInstance()->LoadTexture("res//textures//steam_warship.png"));
		break;

	case Warship:
		SetTexture(AssetManager::GetInstance()->LoadTexture("res//textures//modern_warship.png"));
		break;

	default:
		SetTexture(AssetManager::GetInstance()->LoadTexture("res//textures//raft.png"));
		break;
	}

	setOrigin(GetSize() * 0.5f);
}
