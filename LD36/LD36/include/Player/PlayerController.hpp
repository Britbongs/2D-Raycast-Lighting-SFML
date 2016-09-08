#pragma once

#include "Boat\Boat.hpp"
#include "Projectile\ProjectileManager.hpp"

class PlayerController
{
public:

	explicit PlayerController(ProjectileManager* ProjMgr);

	~PlayerController();

	//Add a pointer to each boat type to the players boat list
	void AddPlayerBoats(Boat* Boat[BOAT_TYPE_COUNT]);

	void Fire(Vector2f MousePos);

	// 0 = no movement, 1 = right, -1 = left
	void Move(Int32 Direc);

	//Will update players timers & players position
	void UpdatePlayer(float Delta);

	//Get a pointer to the active boat to allow the camera to be centred on it 
	const Boat* const GetActiveBoat() const
	{
		assert(ActiveIndex_ >= 0 && ActiveIndex_ < (signed)Boats_.size());

		return Boats_[ActiveIndex_];
	}

	//Get the total number of kills the player has
	Int32 GetKillCount() const { return KillCount_; };

private:

	//Players total number of kills 
	Int32 KillCount_ = 0;

	//Number of lives the player has
	Int32 PlayerLives_ = 0;

	//Pointers to each boat type the player can use
	std::vector<Boat*> Boats_;

	// Projectile manager to allow the boat to fire
	ProjectileManager* const ProjectileMgr_ = nullptr;

	//Index of the boat the player is currently using 
	Int32 ActiveIndex_ = INDEX_NONE; //Active boat index

	//Players movement vector
	Vector2f MoveVec_ = Vector2f(0.f, 0.f);

	//Start the player on a raft
	BoatType CurrentBoat_ = Raft;

	//Texture pointers 
	Texture* BoatTextures_[BOAT_TYPE_COUNT];

	//Asset Manager pointer
	AssetManager* AssetMgr_ = nullptr;
};
