#include "stdafx.hpp"
#include "PlayerController\PlayerController.hpp"


PlayerController::PlayerController(World* ActiveWorld, RenderWindow* RWindow, RenderTarget* RMainTarget)
	: Controller(ActiveWorld, RWindow, RMainTarget)
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

	MovementVector = Normalise(MovementVector) * PLAYER_MOVE_SPEED * Delta;
	WorldIntersectionData IntersectionData = GetWorld()->CheckWorldIntersection(*GetGameObject(), MovementVector);

	if (IntersectionData.bDidIntersect)
	{
		MovementVector = IntersectionData.CollisionResponse;
	}

	Obj->move(MovementVector);

	UpdateRotation();
}

void PlayerController::UpdateRotation()
{
	auto RW = GetRenderWindow(); //Render window 
	auto RT = GetMainRenderTarget(); // active render target
	auto GO = GetGameObject(); // game object controller manipulates 

	Vector2i MouseScreenPos = Mouse::getPosition(*RW);
	Vector2f MouseWorldPos = RT->mapPixelToCoords(MouseScreenPos);

	Vector2f DirectionFromPlayer = GO->getPosition() - MouseWorldPos;

	float RotationAngle = Degrees(atan2(DirectionFromPlayer.y, DirectionFromPlayer.x)); //Angle to rotate player by (in degrees)

	GO->setRotation(RotationAngle);
}
