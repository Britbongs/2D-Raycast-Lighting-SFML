#include "stdafx.h"
#include "Projectile\ProjectileManager.hpp"
#include "Boat\Boat.hpp"
#include "World\World.h"

ProjectileManager::ProjectileManager(World* W)
	: World_(W)
{
	AssetMgr_ = AssetManager::GetInstance();

	Textures_[Rock] = AssetMgr_->LoadTexture("res//textures//rock.png");

	Textures_[Cannonball] = AssetMgr_->LoadTexture("res//textures//cannonball.png");

	Textures_[Missile] = AssetMgr_->LoadTexture("res//textures//missile.png");

	ProjectileSpeeds_ = new float[PROJECTILE_TYPE_COUNT];

	assert(ProjectileSpeeds_);

	ProjectileSpeeds_[Rock] = 700.f;

	ProjectileSpeeds_[Cannonball] = 720.f;

	ProjectileSpeeds_[Missile] = 740.f;
}

ProjectileManager::~ProjectileManager()
{
	delete[] ProjectileSpeeds_;
	ProjectileSpeeds_ = nullptr;
}

bool ProjectileManager::SetupProjectiles()
{
	Int32 Type = (Int32)Rock;
	Int32 Count = 0;

	for (auto& P : Projectiles_)
	{
		if (Count % (ProjectileCount_ / PROJECTILE_TYPE_COUNT) == 0 && Count > 0)
		{
			++Type;
		}

		P->FiredFromState = None;
		P->ProjectileType = (ProjectileType)Type;
		P->SetSize(Vector2f(Textures_[Type]->getSize()));
		P->setOrigin(P->GetSize() * 0.5f);
		P->SetTexture(Textures_[Type]);

	}
	return (true);
}

void ProjectileManager::AddProjectile(Projectile * P)
{
	Projectiles_.push_back(P);
}

void ProjectileManager::FireProjectile(const ProjectileFireData& Data)
{
	/*
	Todo: set projectile as active, set the rotation of the body, set start position, set fired by
	*/
	ProjectileType Type = BoatTypeToProjectileType(Data.BoatType);
	const Int32 Index = GetSpareProjectileIndex(Type);

	if (Index == INDEX_NONE)
	{
		return;
	}
	float theta = atan2(Data.Direction.y, Data.Direction.y);

	auto P = Projectiles_[Index];

	P->InUse = true;
	P->FiredFromState = Data.FiredBy;
	P->setPosition(Data.StartPos);
	P->setRotation(Degrees(theta));
	P->Velocity = Data.Direction * ProjectileSpeeds_[Type];
}

void ProjectileManager::ProjectileUpdate(float Delta)
{
	for (auto P : Projectiles_)
	{
		if (!P->InUse)
		{
			continue;
		}

		P->move(P->Velocity * Delta);

		printf("%f - %f\n", P->getPosition().x, P->getPosition().y);

		if (!World_->IsInsideView(P->GetAABB()))
		{ //if out of bounds
			P->InUse = false;
			P->FiredFromState = None;
			continue;
		}

		CollidedWith With = World_->CheckCollision(P->GetAABB());
		if (With == Player_Boat || With == AI_Boat)
		{
			P->InUse = false;
			P->FiredFromState = None;
		}
	}
}

void ProjectileManager::draw(sf::RenderTarget & RTarget, sf::RenderStates RStates) const
{
	for (auto P : Projectiles_)
	{
		if (!P->InUse)
		{
			continue;
		}
		RTarget.draw(*P);
	}
}

ProjectileType ProjectileManager::BoatTypeToProjectileType(BoatType Type) const
{
	if (Type == Raft || Type == Canoe)
	{
		return Rock;
	}

	if (Type == SailBoat || Type == SteamBoat)
	{
		return Cannonball;
	}

	return Missile;
}

Int32 ProjectileManager::GetSpareProjectileIndex(ProjectileType Type) const
{
	//start the search for a free projectile starting at the first index where that type is located 
	for (Int32 i = (Int32)Type * PROJECTILE_PER_TYPE; i < ProjectileCount_; ++i)
	{
		if (Projectiles_[i]->ProjectileType == Type && !Projectiles_[i]->InUse)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
