#pragma once

#include "Projectile\ProjectileManager.hpp"
#include "World\World.h"

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

	explicit PlayerController(World*);

	~PlayerController();

private:
	//Pointer to active world
	World* World_ = nullptr;
};
