#pragma once

#include "Asset\AssetManager.hpp"
#include "GameObject\GameObject.h"
#include "GameVars.hpp"

//Forward declerations

enum BoatType;
enum BoatControlState;
enum ObjectType;

class Boat;
class World;

//

struct ProjectileFireData
{
	Vector2f StartPos;
	Vector2f Direction;
	BoatControlState FiredBy;
	BoatType BoatType;
};

enum ProjectileType
{//Game projectile types
	eRock,
	eCannonball,
	eMissile
};

//Projectile struct
struct Projectile : public GameObject

{
	Projectile();
	sf::Vector2f Velocity = sf::Vector2f(0.f, 0.f);
	ProjectileType ProjectileType = eRock;
	BoatControlState FiredFromState;
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

	ProjectileType BoatTypeToProjectileType(BoatType Type) const;

	Int32 GetSpareProjectileIndex(ProjectileType Type) const;

	const Int32 ProjectileCount_ = (PROJECTILE_TYPE_COUNT * PROJECTILE_PER_TYPE);

	std::vector<Projectile*> Projectiles_;

	sf::Texture* Textures_[5];

	float* ProjectileSpeeds_ = nullptr;

	AssetManager* AssetMgr_ = nullptr;

	World* World_ = nullptr;
};

