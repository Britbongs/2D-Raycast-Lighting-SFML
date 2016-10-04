#pragma once

#include "Asset\AssetManager.hpp"
#include "GameObject\GameObject.h"
#include "GameVars.hpp"


//Forward decleration
class World;

struct ProjectileFireData
{
	Vector2f StartPos;
	Vector2f Direction;
};


//Projectile struct
struct Projectile : public GameObject

{
	Projectile();
};


class ProjectileManager
{

public:

	ProjectileManager(World* W);

	~ProjectileManager();

	//Will initialise uninitialised projectiles game objects with a set number of projectiles of each type
	bool SetupProjectiles();

	//Add a projectile to the list of available ones
	void AddProjectile(Projectile* P);

	//Fire a projectile using data provided
	void FireProjectile(const ProjectileFireData& Data);

	//Update all projectiles in game
	void ProjectileUpdate(float Delta);

private:


	std::vector<Projectile*> Projectiles_;

	sf::Texture* Textures_[5];

	float* ProjectileSpeeds_ = nullptr;
		
	AssetManager* AssetMgr_ = nullptr;

	World* World_ = nullptr;
};

