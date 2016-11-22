#include "stdafx.hpp"
#include "Controller\Controller.h"


Controller::Controller(World* W, RenderWindow* RWindow, RenderTarget* RMainTarget)
	: World_(W), RWindow_(RWindow), RMainTarget_(RMainTarget)
{
}


Controller::~Controller()
{
}
