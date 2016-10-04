#include "stdafx.h"
#include "DerivedStates\PlayState.hpp"

using namespace sf;

PlayState::PlayState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture)
	: State("Play", ID, RWindow, RTexture)
{
	AssetMgr_ = AssetManager::GetInstance();
}

PlayState::~PlayState()
{

}

bool PlayState::Initialise()
{
	InitRand();

	auto AM = AssetManager::GetInstance();

	World_ = new World(GameObjects_, GetRenderTexture());

	ProjectileMgr_ = new ProjectileManager(World_);

	//Player_ = new PlayerController(ProjectileMgr_, World_, Vector2f(GetRenderTexture()->getSize()));

#ifndef PLAYABLE_BUILD
	assert(ProjectileMgr_);
	assert(World_);
	assert(Player_);
#endif 

	return false;
}

void PlayState::Deinitialise()
{
	if (ProjectileMgr_)
	{
		delete ProjectileMgr_;
		ProjectileMgr_ = nullptr;
	}
	if (Player_)
	{
		delete Player_;
		Player_ = nullptr;
	}
	for (Int32 i = 0; i < (Int32)GameObjects_.size(); ++i)
	{
		if (GameObjects_[i] != nullptr)
		{
			delete GameObjects_[i];
			GameObjects_[i] = nullptr;
		}
	}
}

void PlayState::Update(float Delta)
{

}

void PlayState::Render() const
{
	for (const auto& GO : GameObjects_)
	{
		if (GO->IsActive() == true)
		{
			GetRenderTexture()->draw(*GO);
		}
	}

}

void PlayState::PostRender() const
{

}

void PlayState::HandleEvents(sf::Event& Evnt, float Delta)
{
	if (Evnt.type == sf::Event::KeyPressed)
	{
	}
}
