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

	virtual void Update(float Delta) override;

	virtual void Render() const override;

	virtual void PostRender()const override;

	virtual void HandleEvents(sf::Event& Evnt, float Delta) override;

private:

	AssetManager* AssetMgr_ = AssetManager::GetInstance();

	sf::Texture *SplashTex_ = nullptr, *TitleTex_ = nullptr;
	sf::Sprite Splash_, Title_;

	sf::Time SplashTimer_;

	bool ShowSplash_ = true;

};

