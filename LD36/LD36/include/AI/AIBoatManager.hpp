#pragma once

#include "Asset\AssetManager.hpp"
#include "Boat\Boat.hpp"
#include "Projectile\ProjectileManager.hpp"
#include "World\World.h"
class AIBoatManager
{

public:

	AIBoatManager(ProjectileManager* ProjMgr);

	~AIBoatManager();

	void InitialiseAI(const View& V);

	void AddBoat(Boat* B);

	void Update(float Delta, const View& V);

private:

	bool ValidIndex(Int32 Index) const;

	BoatType GetNextBoatType(BoatType Type) const;

	void SpawnAIPosition(Boat* B, const View& V);

	void SetCurrentBoatType(BoatType Type);

	//Pass the current view and the maximum y value of the object ( Y + Height )
	bool IsOutsideView(const View& V, float YPos)const;

	std::vector<Boat*> Boats_;

	AssetManager* AssetMgr_ = nullptr;

	Texture* BoatTextures_[BOAT_TYPE_COUNT];

	Texture* BuoyTexture_ = nullptr;

	BoatType CurrentBoatType_;
	
	ProjectileManager* ProjMgr_ = nullptr;

};
