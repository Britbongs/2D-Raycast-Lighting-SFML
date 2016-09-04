#include "stdafx.h"
#include "Boat\AIBoatGameLogic.hpp"


AIBoatGameLogic::AIBoatGameLogic(std::vector<Boat>& BoatList)
	: ActiveAIBoats_(5, nullptr)
{

	//Load textures on construction
	BoatTextures_[BoatType::Raft] = AsstMgr_->LoadTexture("res//textures//raft.png");

	BoatTextures_[BoatType::RowingBoat] = AsstMgr_->LoadTexture("res//textures//rowboat.png");

	BoatTextures_[BoatType::SailBoat] = AsstMgr_->LoadTexture("res//textures//sailboat.png");

	BoatTextures_[BoatType::SteamBoat] = AsstMgr_->LoadTexture("res//textures//steam_warship.png");

	BoatTextures_[BoatType::Warship] = AsstMgr_->LoadTexture("res//textures//modern_warship.png");
}


AIBoatGameLogic::~AIBoatGameLogic()
{
	for (Int32 i = 0; i < BOAT_TYPE_COUNT; ++i)
	{
		BoatTextures_[i] = nullptr;
	}
}

void AIBoatGameLogic::InitialiseAI(BoatType StartType)
{
	//initialise AI boat types
	Int32 AITypeInt = 0;
	BoatType AIType = StartType;

	for (Int32 i(0); i < (signed)AIBoats_.size(); ++i)
	{
		auto& B = *AIBoats_[i];

		if (i % 6 == 0 && i > 0)
		{
			++AITypeInt;
			AIType = (BoatType)AIType;
		}
	}
}

void AIBoatGameLogic::SetAIBoat(Boat* B)
{
	assert(B != nullptr);

	AIBoats_.push_back(B);
}

bool AIBoatGameLogic::ValidIndex(Int32 Index) const
{
	return Index >= 0 && Index < AIBoats_.size();
}
