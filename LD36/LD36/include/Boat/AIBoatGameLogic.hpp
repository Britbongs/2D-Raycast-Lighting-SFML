#pragma once

#include "Asset\AssetManager.hpp"
#include "Boat\Boat.hpp"

class AIBoatGameLogic
{

public:

	AIBoatGameLogic(std::vector<Boat>& BoatList);

	~AIBoatGameLogic();

	void InitialiseAI(BoatType StartType = BoatType::Raft);

	void SetAIBoat(Boat* B);

private:

	bool ValidIndex(Int32 Index) const;

	std::vector<Boat*> AIBoats_;

	std::vector<Boat*> ActiveAIBoats_;

	AssetManager* AsstMgr_ = nullptr;

	Texture* BoatTextures_[BOAT_TYPE_COUNT];

};

