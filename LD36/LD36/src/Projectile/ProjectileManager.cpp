#include "stdafx.h"
#include "Projectile\ProjectileManager.hpp"
#include "Boat\Boat.hpp"
#include "World\World.h"


//PROJECTILE CTOR
Projectile::Projectile()
	:GameObject(ObjectType::eProjectile)
{

}
//
ProjectileManager::ProjectileManager(World* W)
	: World_(W)
{
	AssetMgr_ = AssetManager::GetInstance();

	Textures_[eRock] = AssetMgr_->LoadTexture("res//textures//rock.png");

	Textures_[eCannonball] = AssetMgr_->LoadTexture("res//textures//cannonball.png");

	Textures_[eMissile] = AssetMgr_->LoadTexture("res//textures//missile.png");

	ProjectileSpeeds_ = new float[PROJECTILE_TYPE_COUNT];

	assert(ProjectileSpeeds_);

	ProjectileSpeeds_[eRock] = 700.f;

	ProjectileSpeeds_[eCannonball] = 720.f;

	ProjectileSpeeds_[eMissile] = 740.f;
}

ProjectileManager::~ProjectileManager()
{
	delete[] ProjectileSpeeds_;
	ProjectileSpeeds_ = nullptr;
}

bool ProjectileManager::SetupProjectiles()
{
	Int32 Type = (Int32)eRock;
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
		P->SetInActive();
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

	P->FiredFromState = Data.FiredBy;
	P->setPosition(Data.StartPos);
	P->setRotation(Degrees(theta));
	P->Velocity = Data.Direction * ProjectileSpeeds_[Type];
	P->SetActive();
}

void ProjectileManager::ProjectileUpdate(float Delta)
{
	for (auto P : Projectiles_)
	{
		if (P->IsActive() == false)
		{
			continue;
		}

		P->move(P->Velocity * Delta);

		printf("%f - %f\n", P->getPosition().x, P->getPosition().y);

		if (!World_->IsInsideView(P->GetAABB()))
		{ //if out of bounds
			P->SetInActive();
			P->FiredFromState = None;
			continue;
		}

		CollisionData CollidedWith = World_->CheckCollision(*P);

		if (CollidedWith.ObjType == ePlayerBoat || CollidedWith.ObjType == eAIBoat)
		{
			P->FiredFromState = None;
			P->SetInActive();
		}
	}
}

ProjectileType ProjectileManager::BoatTypeToProjectileType(BoatType Type) const
{
	if (Type == Raft || Type == Canoe)
	{
		return eRock;
	}

	if (Type == SailBoat || Type == SteamBoat)
	{
		return eCannonball;
	}

	return eMissile;
}

Int32 ProjectileManager::GetSpareProjectileIndex(ProjectileType Type) const
{
	//start the search for a free projectile starting at the first index where that type is located 
	for (Int32 i = (Int32)Type * PROJECTILE_PER_TYPE; i < ProjectileCount_; ++i)
	{
		if (Projectiles_[i]->ProjectileType == Type && Projectiles_[i]->IsActive() == false)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
