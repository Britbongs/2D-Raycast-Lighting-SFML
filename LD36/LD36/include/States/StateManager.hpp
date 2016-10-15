#pragma once

#include "States\State.hpp"

/*
	-State manager which will handle allocation & deallocation of game states at initialisation & destruction stages of the engine
	(Allows for dynamic state creation if needed)

	-TODO: Remove raw pointers & implement smart pointers
	-TODO: Possibly required public state id based functions
	-TODO: Replace event params to a pointer to static input handler

*/

class StateManager
{
public:

	StateManager();

	~StateManager();
	//Initialises all states in the game 
	void InitialiseAllStates();

	void CleanupStates();

	//Initialises a specific state
	void InitialiseState(const sf::String& StateName);

	void ActiveStateHandleEvents(sf::Event& Evnt, float Delta);

	void ActiveStateUpdate(float Delta);

	void ActiveStateRender() const;

	void ActiveStatePostRender() const;

	void AddState(State* State, bool InUse = true);

	//Sets a state that is currently in use to being active
	//Any state marked as not in use will be ignored
	void SetActiveState(const sf::String& StateName);

	//Will return a pointer to the state that is currently being executed
	State* const GetActiveState();

private:

	bool IsValidStateID(Int32 StateID) const;

	//Will return a state found by it's name, will return nullptr if nothing is found
	//Make sure to check return
	State* const GetStateByName(const sf::String& StateName);

	//Function to find state ID via name, will return -1 if nothing is found
	Int32 GetStateID(const sf::String& StateName) const;

	//Data Member
	std::vector<State*> States_;

	Int32 ActiveStateID_ = 0;

};

