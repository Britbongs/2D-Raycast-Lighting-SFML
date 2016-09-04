#include "stdafx.h"
#include "Game\Game.hpp"
#include "Asset\AssetManager.hpp"
#include <future>

using namespace sf;

Game::Game()
{
}


Game::~Game()
{
}

bool Game::InitGame()
{
	auto AM = AssetManager::GetInstance();
	if (AM->GetDefaultFont() == nullptr)
	{
		PrintToDebug("Error: ", "Failed To Load FPS Font");
		return false;
	}

	if (!InitialiseWindow())
	{
		return false;
	}

	FPSText_.setFont(*AM->GetDefaultFont());
	FPSText_.setCharacterSize(12u);
	FPSText_.setColor(sf::Color::Yellow);
	FPSText_.setString(L"FPS:  ");
	FPSText_.setPosition(WindowDimensions.x - (FPSText_.getGlobalBounds().width * 2.f), 10);

	auto b = std::async(std::launch::async, &Game::InitialiseStates, this);
	if (!b.get())
	{
		return false;
	}
	return true;
}

void Game::RunGame()
{
	Clock DeltaClock;
	ElapsedClock.restart();

	while (RWindow_.isOpen())
	{
		sf::Event Evnt;

		float Delta = DeltaClock.restart().asSeconds();

		while (RWindow_.pollEvent(Evnt))
		{
			if (Evnt.type == sf::Event::Closed)
			{
				RWindow_.close();
			}
			HandleEvents(Evnt, Delta);
		}
	
		Update(Delta);

		Render(Delta);

	}
}

void Game::CleanUpGame()
{
	auto p = AssetManager::GetInstance();
	delete p;
}
// Private Function


bool Game::InitialiseWindow()
{
	//Initialise RenderWindow
	RWindow_.create(sf::VideoMode(WindowDimensions.x, WindowDimensions.y), GameWindowTitle_, sf::Style::Close);

	//Initialise RenderTexture
	if (!RTexture_.create(WindowDimensions.x, WindowDimensions.y, false))
	{
		return false;
	}

	auto AM = AssetManager::GetInstance();
	if (AM->GetDefaultFont() == nullptr)
	{
		PrintToDebug("Error: ", "Failed To Load FPS Font");
		return false;
	}

	Texture* Tex = AM->LoadTexture("res//textures//splash.png");
	if (Tex != nullptr)
	{
		Image IconImage = Tex->copyToImage();

		RWindow_.setIcon(IconImage.getSize().x, IconImage.getSize().y, IconImage.getPixelsPtr());

	}
	RWindow_.setFramerateLimit(60);
	return true;
}

bool Game::InitialiseStates()
{
	StateManager_.AddState(new PlayState(0, &RWindow_, &RTexture_));
	StateManager_.AddState(new MenuState(1, &RWindow_, &RTexture_));

	StateManager_.InitialiseAllStates();
	StateManager_.SetActiveState("Play");

	return true;
}

void Game::Update(float Delta)
{
	UpdateFPSCounter(Delta);

	StateManager_.ActiveStateUpdate(Delta);

}

void Game::HandleEvents(sf::Event & Evnt, float Delta)
{
#if IN_DEVELOPMENT_BUILD
	if (Evnt.type == sf::Event::KeyPressed)
	{
		if (Evnt.key.code == sf::Keyboard::Escape)
		{
			RWindow_.close();
		}
	}
	StateManager_.ActiveStateHandleEvents(Evnt, Delta);
#endif
}

void Game::Render(float Delta)
{
	RTexture_.clear(Color::Black);
	StateManager_.ActiveStateRender();
	RTexture_.display();

	const sf::Texture& Buffer(RTexture_.getTexture());
	sf::Sprite ToBeDrawn(Buffer);

	RWindow_.clear(Color::Black);

	RWindow_.draw(ToBeDrawn);
	RWindow_.draw(FPSText_);
	StateManager_.ActiveStatePostRender();
	RWindow_.display();
}

void Game::UpdateFPSCounter(float Delta)
{
	const float Elapsed(ElapsedClock.getElapsedTime().asSeconds());
	if (First_)
	{
		Frames_ = 0;
		FPS_ = 1.f / Delta;
		StartTime_ = Elapsed;
		First_ = false;

	}

	++Frames_;


	if (Elapsed - StartTime_ > 0.25f && Frames_ > 10)
	{
		FPS_ = (float)Frames_ / (Elapsed - StartTime_);
		StartTime_ = Elapsed;
		Frames_ = 0;
	}


	FPSText_.setString("FPS: " + sf::String(std::to_string((int)FPS_)));

}