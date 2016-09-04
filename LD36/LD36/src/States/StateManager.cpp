#include "stdafx.h"
#include "States\StateManager.hpp"

using namespace std;

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
	for (auto S : States_)
	{
		delete S;
		S = nullptr;
	}
}

void StateManager::InitialiseAllStates()
{
	for (auto S : States_)
	{
		assert(S);
		S->Initialise();
	}
}

void StateManager::InitialiseState(const sf::String & StateName)
{
	State* const S = GetStateByName(StateName);
	if (S != nullptr)
	{
		S->Initialise();
	}
}

void StateManager::ActiveStateHandleEvents(sf::Event& Evnt, float Delta)
{
	States_[ActiveStateID_]->HandleEvents(Evnt, Delta);
}

void StateManager::ActiveStateUpdate(float Delta)
{
	if (States_[ActiveStateID_]->IsFinished())
	{
		ActiveStateID_ = GetStateID(States_[ActiveStateID_]->GetNextState());
	}
	//std::cout << ActiveStateID_ << std::endl;
	States_[ActiveStateID_]->Update(Delta);
}

void StateManager::ActiveStateRender() const
{
	States_[ActiveStateID_]->Render();
}

void StateManager::ActiveStatePostRender() const
{
	States_[ActiveStateID_]->PostRender();
}

void StateManager::AddState(State* NewState, bool InUse)
{
	assert(NewState);

	States_.push_back(NewState);
}

void StateManager::SetActiveState(const sf::String& StateName)
{
	Int32 StateID = GetStateID(StateName);
	if (IsValidStateID(StateID))
	{
		ActiveStateID_ = StateID;
	}
}

State* const StateManager::GetActiveState()
{
	if (States_.size() > 0)
		return States_[ActiveStateID_];

	return(nullptr);
}

bool StateManager::IsValidStateID(Int32 StateID) const
{
	return (StateID >= 0 && StateID < (signed)States_.size());
}

State* const StateManager::GetStateByName(const sf::String& StateName)
{
	Int32 StateID = GetStateID(StateName);

	if (IsValidStateID(StateID))
	{
		return(States_[StateID]);
	}

#if IN_DEVELOPMENT_BUILD
	std::wcout << L"Error! Couldn't find state (named: " << StateName.toWideString() << L")" << std::endl;
#endif
	return(nullptr);
}

Int32 StateManager::GetStateID(const sf::String & StateName) const
{
	for (Int32 i(0); i < (signed)States_.size(); ++i)
	{
		if (States_[i]->GetStateName() == StateName)
		{
			std::wcout << StateName.toWideString() << L" "<< StateName.toWideString() << std::endl;
			return(i);
		}
	}
	return Int32(INDEX_NONE);
}
