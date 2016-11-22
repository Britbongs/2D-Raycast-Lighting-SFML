#pragma once

#include "Projectile\ProjectileManager.hpp"
#include "World\World.hpp"
#include "Controller\Controller.h"

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

class PlayerController : public Controller
{
public:

	explicit PlayerController(World* ActiveWorld, RenderWindow* RWindow, RenderTarget* RMainTarget);

	~PlayerController();

	virtual void Initialise() override;

	virtual void Deinitialise() override;

	virtual void Update(float Delta) override;

private:

	void UpdateRotation();

	//Pointer to active world
	World* World_ = nullptr;
};
