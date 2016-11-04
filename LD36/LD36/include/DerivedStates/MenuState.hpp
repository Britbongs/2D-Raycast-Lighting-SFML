#pragma once
#include "States\State.hpp"	
#include "Asset\AssetManager.hpp"

class MenuState
	: public State
{
public:
	MenuState(sf::Int32 ID, sf::RenderWindow* RWindow, sf::RenderTexture* RTexture);

	~MenuState();

	virtual bool Initialise() override;

	virtual void Deinitialise()	 override;

	virtual void Update(float Delta) override;

	virtual void Render() override;

	virtual void PostRender() const override;

	virtual void HandleEvents(sf::Event& Evnt, float Delta) override;

private:

};

