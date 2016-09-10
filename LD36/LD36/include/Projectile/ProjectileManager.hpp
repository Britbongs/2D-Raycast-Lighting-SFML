#pragma once

#include "Asset\AssetManager.hpp"
#include "GameVars.hpp"

enum BoatType;
enum BoatControlState;

class Boat;

struct ProjectileFireData
{
	Vector2f StartPos;
	Vector2f Direction;
	BoatControlState FiredBy;
	BoatType BoatType;
};

class ProjectileManager :
	public sf::Drawable
{

private:

	enum ProjectileType
	{
		Rock,
		Cannonball,
		Missile
	};

	struct Projectile
	{
		sf::RectangleShape Body;
		sf::Vector2f Velocity = sf::Vector2f(0.f, 0.f);
		bool InUse = false;
		ProjectileType ProjectileType = Rock;
		BoatControlState FiredFromState;
	};

public:

	ProjectileManager(const sf::RenderTexture* const RTex);

	~ProjectileManager();

	bool SetupProjectiles();

	void FireProjectile(const ProjectileFireData& Data);

	void ProjectileUpdate(float Delta, std::vector<Boat*>& Boats);

private:

	virtual void draw(sf::RenderTarget& RTarget, sf::RenderStates RStates) const override;

	ProjectileType BoatTypeToProjectileType(BoatType Type) const;

	Int32 GetSpareProjectileIndex(ProjectileType Type) const;

	const Int32 ProjectileTypeCount_ = 3;

	const Int32 ProjectilePerType_ = 15;

	const Int32 ProjectileCount_ = (ProjectileTypeCount_ * ProjectilePerType_);

	std::vector<Projectile*> Projectiles_;

	sf::Texture* Textures_[5];

	float* ProjectileSpeeds_ = nullptr;

	const sf::RenderTexture* const RTex_;

	AssetManager* AssetMgr_ = nullptr;

};

