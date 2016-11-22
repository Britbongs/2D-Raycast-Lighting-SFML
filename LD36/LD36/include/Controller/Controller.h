#pragma once

#include "GameVars.hpp"
#include "World\World.hpp"
#include "GameObject\GameObject.h"

class Controller
{

public:

	explicit Controller(World* World, RenderWindow* RWindow, RenderTarget* RMainTarget);

	~Controller();

	virtual void Initialise() = 0;

	virtual void Deinitialise() = 0;

	virtual void Update(float Delta) = 0;

	void SetGameObject(GameObject* Object) { GameObject_ = Object; };

protected:

	GameObject* GetGameObject() const { return GameObject_; }

	World* GetWorld() const { return World_; }

	RenderWindow* GetRenderWindow() const { return RWindow_; }

	RenderTarget* GetMainRenderTarget() const { return RMainTarget_; }
private:

	World* World_ = nullptr;
	GameObject* GameObject_ = nullptr;
	RenderWindow* RWindow_ = nullptr;
	RenderTarget* RMainTarget_ = nullptr;
};

