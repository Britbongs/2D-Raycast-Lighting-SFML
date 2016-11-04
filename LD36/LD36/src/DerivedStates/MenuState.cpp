#include "stdafx.hpp"
#include "DerivedStates\MenuState.hpp"


MenuState::MenuState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture)
	: State("Menu", ID, RWindow, RTexture)
{
}


MenuState::~MenuState()
{
}

bool MenuState::Initialise()
{

	return true;
}

void MenuState::Deinitialise()
{
}

void MenuState::Update(float Delta)
{
}

void MenuState::Render()
{
}

void MenuState::PostRender() const
{
}

void MenuState::HandleEvents(sf::Event & Evnt, float Delta)
{
	if (Evnt.type == Event::KeyPressed)
	{
	}
}
