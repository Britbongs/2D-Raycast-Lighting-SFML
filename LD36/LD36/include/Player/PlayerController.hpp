#pragma once

#include "Boat\Boat.hpp"
#include "Projectile\ProjectileManager.hpp"

enum PlayerMoveDirection
{
	UP,
	UP_LEFT,
	UP_RIGHT
};

/*
TODO:
- Implement attack grace timer
- Implement fire cool down timer
*/

class PlayerController
{
public:

	explicit PlayerController(ProjectileManager* ProjMgr, Vector2f ScreenSize);

	~PlayerController();

	//Add a pointer to each boat type to the players boat list
	void AddPlayerBoats(Boat * B);

	//Fire a projectile 
	void Fire();

	// Set the movement direction of the player 
	void SetDirection(PlayerMoveDirection Move);

	//Will update players timers & players position
	void UpdatePlayer(float Delta);

	//Get a pointer to the active boat to allow the camera to be centred on it 
	const Boat* const GetActiveBoat() const
	{
		return Boats_[CurrentBoat_];
	}

	//Get the total number of kills the player has
	Int32 GetKillCount() const { return KillCount_; };

	//Get the player movement vector 
	Vector2f GetMoveVector() const { return MoveVec_; }

private:

	//Player movement speed 
	const float MOVE_SPEED = 150.f;
	//Players total number of kills 
	Int32 KillCount_ = 0;

	//Number of lives the player has
	Int32 PlayerLives_ = 0;

	//Pointers to each boat type the player can use
	std::vector<Boat*> Boats_;

	// Projectile manager to allow the boat to fire
	ProjectileManager* const ProjectileMgr_ = nullptr;

	//Players movement vector
	Vector2f MoveVec_ = Vector2f(0.f, 0.f);

	//Screen dimensions
	const Vector2f ScreenSize_;

	//Start the player on a raft
	BoatType CurrentBoat_ = Raft;

	Time GraceTimer_ = seconds(0.f);

	Time FireCooldownTimer_ = seconds(0.f);
};
