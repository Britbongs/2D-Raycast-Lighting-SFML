#include "stdafx.h"
#include "AI\AIBoatManager.hpp"


AIBoatManager::AIBoatManager(ProjectileManager* ProjMgr)
	: ProjMgr_(ProjMgr)
{

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

		B = Boat(None, Type);
	}

	SetCurrentBoatType(Warship);

	for (auto B : Boats_)
	{
		if (B->GetControlState() != AI)
		{
			SpawnAIPosition(B, V);
		}
	}
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

		if (IsOutsideView(V, B->getPosition().y - B->GetSize().y / 2.f))
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
	case Raft: Type = Canoe; break;
	case Canoe: Type = SailBoat; break;
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


	const float SpawnDist(800);

	float BeforePoint(V.getCenter().y);
	float XSpawn = RandFloat(B->GetSize().x / 2.f, (float)V.getSize().x - B->GetSize().x);

	BeforePoint -= V.getSize().y;
	BeforePoint = fabs(BeforePoint);

	B->setPosition(XSpawn, RandFloat(BeforePoint, BeforePoint + SpawnDist) * -1);

	for (auto B2 : Boats_)
	{
		if (B2 != B)
		{
			if (GetSquareLength(B2->getPosition() - B->getPosition()) <= pow(SpawnDist, 2))
			{
				XSpawn = RandFloat(0.f, (float)V.getSize().x - B->GetSize().x);
				B->setPosition(XSpawn, RandFloat(BeforePoint, BeforePoint + SpawnDist) * -1);
			}
		}
	}
	printf("%f\n", XSpawn);
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
