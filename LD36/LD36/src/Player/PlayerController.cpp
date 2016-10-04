#include "stdafx.h"
#include "Player\PlayerController.hpp"


PlayerController::PlayerController(World* ActiveWorld)
	: World_(ActiveWorld)
{
#if IN_DEVELOPMENT_BUILD
	assert(ProjectileMgr);
	assert(ActiveWorld);
#endif
}

PlayerController::~PlayerController()
{
}
