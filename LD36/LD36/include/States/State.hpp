#pragma once

using namespace sf;
/*
	-Base class for state interface to be implemented by user
	-TODO Complete base class implementations
*/

class State
{
public:

	State(const sf::String&, Uint32 StateID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture);

	virtual ~State();

	virtual bool Initialise() = 0;
	
	virtual void Deinitialise() = 0;

	virtual void Update(float Delta) = 0;

	virtual void Render() const = 0;

	virtual void PostRender() const = 0;

	virtual void HandleEvents(sf::Event& Evnt, float Delta) = 0;

	void SetNextState(const sf::String& StateName) { NextState_ = StateName; }

	void SetStateFinished() { StateFinished_ = true; }

	bool IsFinished() const { return StateFinished_; }

	sf::String GetStateName() const { return StateName_; }

	sf::String GetNextState() const { return NextState_; }

	Uint32 GetStateID() const { return StateID_; }

protected:

	sf::RenderWindow * const GetRenderWindow() { return RWindow_; }
	sf::RenderWindow * const GetRenderWindow() const { return RWindow_; }
	sf::RenderTexture* const GetRenderTexture() { return RTexture_; }
	sf::RenderTexture* const GetRenderTexture() const { return RTexture_; }

private:

	sf::RenderWindow* RWindow_ = nullptr;
	sf::RenderTexture* RTexture_ = nullptr;

	const sf::String StateName_;
	const Uint32 StateID_;

	sf::String NextState_ = " ";
	bool StateFinished_ = false;
};
