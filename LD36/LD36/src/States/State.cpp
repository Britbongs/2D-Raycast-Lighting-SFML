#include "stdafx.h"
#include "States\State.hpp"


State::State(const sf::String& StateName, Uint32 StateID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture)
	: StateName_(StateName), StateID_(StateID), RWindow_(RWindow), RTexture_(RTexture)
{
}

State::~State()
{
}
