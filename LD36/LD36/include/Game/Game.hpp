#pragma once

#include "States\StateManager.hpp"
#include "DerivedStates\PlayState.hpp"
#include "DerivedStates\MenuState.hpp"

class Game
{
public:

	Game();

	~Game();

	bool InitGame();

	void RunGame();

	void CleanUpGame();

private:	

	bool InitialiseWindow();

	bool InitialiseStates();

	void Update(float Delta);

	void HandleEvents(sf::Event& Evnt, float Delta);

	void Render(float Delta);
	
	void UpdateFPSCounter(float Delta);

	sf::RenderWindow RWindow_;
	sf::RenderTexture RTexture_;

	StateManager StateManager_;

	Clock ElapsedClock;
	
	Text FPSText_;

	float StartTime_ = 0.f;
	sf::Int32 Frames_ = 0;
	bool First_ = true;
	float FPS_ = 0.f;


	bool CompletedAverages_ = false; //Has completed 1 pass through the averages array 

	const sf::String GameWindowTitle_ = "Get Dat Boaty - LD36";
	const sf::Vector2u WindowDimensions = sf::Vector2u(1024, 768);
};

