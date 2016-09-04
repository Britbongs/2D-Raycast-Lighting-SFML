#include "stdafx.h"
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
	SplashTex_ = AssetMgr_->LoadTexture("res//textures//splash.png");
	TitleTex_ = AssetMgr_->LoadTexture("res//textures//title.png");

	Splash_ = Sprite(*SplashTex_);
	Title_ = Sprite(*TitleTex_);

	return true;
}

void MenuState::Update(float Delta)
{
	SplashTimer_ += Time(seconds(Delta));
	if (SplashTimer_ > Time(seconds(2.5f)))
	{
		ShowSplash_ = false;
	}
}

void MenuState::Render() const
{
}

void MenuState::PostRender() const
{
	if (ShowSplash_)
	{
		GetRenderWindow()->draw(Splash_);
	}
	else
	{
		GetRenderWindow()->draw(Title_);
	}
}

void MenuState::HandleEvents(sf::Event & Evnt, float Delta)
{
	if (Evnt.type == Event::KeyPressed)
	{
		if (Evnt.key.code == Keyboard::X)
		{
			if (!ShowSplash_)
			{
				SetNextState("Play");
				SetStateFinished();
			}
		}
	}
}
