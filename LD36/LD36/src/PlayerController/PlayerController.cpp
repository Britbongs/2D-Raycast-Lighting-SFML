#include "stdafx.hpp"
#include "PlayerController\PlayerController.hpp"


PlayerController::PlayerController(World* ActiveWorld)
	: Controller(ActiveWorld)
{
#if IN_DEVELOPMENT_BUILD
	assert(ProjectileMgr);
	assert(ActiveWorld);
#endif
}

PlayerController::~PlayerController()
{
}

void PlayerController::Initialise()
{
	auto Obj = GetGameObject();
	Obj->setPosition(200, 200);
	Obj->SetColour(Colour::Blue);
}

void PlayerController::Deinitialise()
{
}

void PlayerController::Update(float Delta)
{
	auto Obj = GetGameObject();

	Vector2f MovementVector(0.f, 0.f);

	if (Keyboard::isKeyPressed(Keyboard::Key::A))
	{
		MovementVector.x = -1.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::D))
	{
		MovementVector.x = 1.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::W))
	{
		MovementVector.y = -1.f;

	}

	if (Keyboard::isKeyPressed(Keyboard::Key::S))
	{
		MovementVector.y = 1.f;

	}

	MovementVector = Normalise(MovementVector);

	Obj->move(Delta * MovementVector * PLAYER_MOVE_SPEED);
}
