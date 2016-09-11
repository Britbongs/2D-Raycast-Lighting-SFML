#pragma once

#include "Asset\AssetManager.hpp"
#include "GameObject\GameObject.h"
#include "GameVars.hpp"

enum BoatType;
enum BoatControlState;

class Boat;
class World;

struct ProjectileFireData
{
	Vector2f StartPos;
	Vector2f Direction;
	BoatControlState FiredBy;
	BoatType BoatType;
};

enum ProjectileType
{
	Rock,
	Cannonball,
	Missile
};

struct Projectile : public GameObject
{
	sf::Vector2f Velocity = sf::Vector2f(0.f, 0.f);
	bool InUse = false;
	ProjectileType ProjectileType = Rock;
	BoatControlState FiredFromState;
};


class ProjectileManager :
	public sf::Drawable
{

public:

	ProjectileManager(World* W);

	~ProjectileManager();

	bool SetupProjectiles();

	void AddProjectile(Projectile* P);

	void FireProjectile(const ProjectileFireData& Data);

	void ProjectileUpdate(float Delta);

private:

	virtual void draw(sf::RenderTarget& RTarget, sf::RenderStates RStates) const override;

	ProjectileType BoatTypeToProjectileType(BoatType Type) const;

	Int32 GetSpareProjectileIndex(ProjectileType Type) const;

	const Int32 ProjectileCount_ = (PROJECTILE_TYPE_COUNT * PROJECTILE_PER_TYPE);

	std::vector<Projectile*> Projectiles_;

	sf::Texture* Textures_[5];

	float* ProjectileSpeeds_ = nullptr;

	AssetManager* AssetMgr_ = nullptr;

	World* World_ = nullptr;
};

