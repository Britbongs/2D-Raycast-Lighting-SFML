#pragma once

#include "GameVars.hpp"
#include "World\World.hpp"
#include "GameObject\GameObject.h"

class Controller
{

public:

	explicit Controller(World* World);

	~Controller();

	virtual void Initialise() = 0;

	virtual void Deinitialise() = 0;

	virtual void Update(float Delta) = 0;

	void SetGameObject(GameObject* Object) { GameObject_ = Object; };

protected:

	GameObject* GetGameObject() { return GameObject_; }

	World* GetWorld() { return World_; }

private:

	World* World_ = nullptr;
	GameObject* GameObject_ = nullptr;
};

