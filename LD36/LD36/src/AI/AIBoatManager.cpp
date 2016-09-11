#include "stdafx.h"
#include "AI\AIBoatManager.hpp"


AIBoatManager::AIBoatManager(ProjectileManager* ProjMgr)
	: ProjMgr_(ProjMgr)
{
	AssetMgr_ = AssetManager::GetInstance();
	//Load textures on construction
	BoatTextures_[BoatType::Raft] = AssetMgr_->LoadTexture("res//textures//raft.png");
	BoatTextures_[BoatType::RowingBoat] = AssetMgr_->LoadTexture("res//textures//rowboat.png");
	BoatTextures_[BoatType::SailBoat] = AssetMgr_->LoadTexture("res//textures//sailboat.png");
	BoatTextures_[BoatType::SteamBoat] = AssetMgr_->LoadTexture("res//textures//steam_warship.png");
	BoatTextures_[BoatType::Warship] = AssetMgr_->LoadTexture("res//textures//modern_warship.png");

	BuoyTexture_ = AssetMgr_->LoadTexture("res//textures//buoy.png");
}


AIBoatManager::~AIBoatManager()
{
	for (Int32 i = 0; i < BOAT_TYPE_COUNT; ++i)
	{
		BoatTextures_[i] = nullptr;
	}
}

void AIBoatManager::InitialiseAI(const View& V)
{
	//initialise AI boat types
	BoatType Type = Raft;
	Int32 AITypeInt = 0;

	for (Int32 i(0); i < (signed)Boats_.size(); ++i)
	{
		auto& B = *Boats_[i];

		if (i % AI_BOAT_PER_TYPE_COUNT == 0 && i > 0)
		{
			++AITypeInt;

			Type = GetNextBoatType(Type);
		}

		B = Boat(ProjMgr_, None, Type);
		B.SetTexture(BoatTextures_[Type], BuoyTexture_);
		B.SetSize(Vector2f(BoatTextures_[Type]->getSize()));
		SpawnAIPosition(&B, V);
	}
	SetCurrentBoatType(Warship);
}

void AIBoatManager::AddBoat(Boat* B)
{
	assert(B != nullptr);

	Boats_.push_back(B);
}

void AIBoatManager::Update(float Delta, const View& V)
{
	for (auto B : Boats_)
	{
		if (B->GetControlState() != AI)
		{
			continue;
		}

		if (IsOutsideView(V, B->getPosition().y - B->GetGlobalBounds().height / 2.f))
		{
			SpawnAIPosition(B, V);
		}
	}
}

// Private functions 

bool AIBoatManager::ValidIndex(Int32 Index) const
{
	return Index >= 0 && Index < (signed)Boats_.size();
}

BoatType AIBoatManager::GetNextBoatType(BoatType Type) const
{
	switch (Type)
	{
	case Raft: Type = RowingBoat; break;
	case RowingBoat: Type = SailBoat; break;
	case SailBoat: Type = SteamBoat; break;
	default: Type = Warship; break;
	}
	return Type;
}

void AIBoatManager::SpawnAIPosition(Boat * B, const View & V)
{
	//TODO spawn boats off camera randomly and spaced out enough 

	const Vector2f LastPos = B->getPosition();
	B->setPosition(V.getCenter() - Vector2f(RandFloat(0.f, V.getSize().x), 200));
}

void AIBoatManager::SetCurrentBoatType(BoatType Type)
{
	for (auto B : Boats_)
	{
		if (B->GetBoatType() != Type && B->GetBoatType() != CurrentBoatType_)
		{//If not the boat we're wanting to setup & not the previous boat
			continue;
		}
		else if (B->GetBoatType() != Type && B->GetBoatType() == CurrentBoatType_)
		{//if not the boat we're looking for, but is the previous boat
			B->SetControlState(None);
			continue;
		}
		B->SetControlState(AI);
	}
	CurrentBoatType_ = Type;
}

bool AIBoatManager::IsOutsideView(const View & V, float YPos) const
{
	//Create a float rect container
	FloatRect ViewBounds;
	ViewBounds.left = V.getCenter().x - V.getSize().x / 2.f;
	ViewBounds.top = V.getCenter().y - V.getSize().y / 2.f;
	ViewBounds.width = V.getSize().x;
	ViewBounds.width = V.getSize().y;

	//Check if the objects y position is off the screen and the objects y position 
	if (YPos > V.getCenter().y + V.getSize().y / 2.f)
	{
		return true;
	}
	return false;
}
