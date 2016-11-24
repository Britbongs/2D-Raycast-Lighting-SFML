#pragma once

#include "Projectile\ProjectileManager.hpp"
#include "World\World.hpp"
#include "Controller\Controller.h"

enum PlayerMoveDirection : Uint32
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

	void PlayerJoystickUpdate(float Delta);

	//Pointer to active world
	World* World_ = nullptr;
	const Int32 PLAYER_JOYSTICK_SLOT = 0; 

	bool bIsJoystickConnected_ = false;

	const float AXIS_DEAD_ZONE = 5.f;
};
