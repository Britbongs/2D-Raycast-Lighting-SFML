#include "stdafx.h"
#include "Projectile\ProjectileManager.hpp"
#include "Boat\Boat.hpp"

ProjectileManager::ProjectileManager(const RenderTexture* const RTex)
	: Projectiles_(ProjectileCount_, nullptr), RTex_(RTex)
{

	Textures_ = new Texture*[ProjectileTypeCount_];
	assert(Textures_);

	AssetMgr_ = AssetManager::GetInstance();

	Textures_[Rock] = AssetMgr_->LoadTexture("res//textures//rock.png");

	Textures_[Cannonball] = AssetMgr_->LoadTexture("res//textures//cannonball.png");

	Textures_[Missile] = AssetMgr_->LoadTexture("res//textures//missile.png");

	ProjectileSpeeds_ = new float[ProjectileTypeCount_];

	assert(ProjectileSpeeds_);

	ProjectileSpeeds_[Rock] = 700.f;
	ProjectileSpeeds_[Cannonball] = 720.f;
	ProjectileSpeeds_[Missile] = 740.f;
}

ProjectileManager::~ProjectileManager()
{
	for (auto P : Projectiles_)
	{
		delete P;
		P = nullptr;
	}

	delete[] Textures_;
	Textures_ = nullptr;

	delete[] ProjectileSpeeds_;
}

bool ProjectileManager::SetupProjectiles()
{
	Int32 Type = (Int32)Rock;
	Int32 Count = 0;

	for (auto& P : Projectiles_)
	{
		P = new Projectile;
		assert(P);

		if (Count % (ProjectileCount_ / ProjectileTypeCount_) == 0 && Count > 0)
		{
			++Type;
		}

		P->FiredFromState = None;
		P->ProjectileType = (ProjectileType)Type;
		P->Body.setSize(Vector2f(Textures_[Type]->getSize()));
		P->Body.setOrigin(P->Body.getLocalBounds().width / 2.f, P->Body.getLocalBounds().height / 2.f);
		P->Body.setTexture(Textures_[Type]);

	}
	return (true);
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
	P->Body.setPosition(Data.StartPos);
	P->Body.setRotation(Degrees(theta));

	P->Velocity = Data.Direction * ProjectileSpeeds_[Type];
}

void ProjectileManager::ProjectileUpdate(float Delta, std::vector<Boat>& Boats)
{
	View V(RTex_->getView());
	FloatRect ViewCollider;

	ViewCollider.left = V.getCenter().x - V.getSize().x / 2.f;
	ViewCollider.top = V.getCenter().y - V.getSize().y / 2.f;
	ViewCollider.width = V.getSize().x;
	ViewCollider.height = V.getSize().y;


	for (auto P : Projectiles_)
	{
		if (!P->InUse)
		{
			continue;
		}

		P->Body.move(P->Velocity * Delta);

		printf("%f - %f\n", P->Body.getPosition().x, P->Body.getPosition().y);

		if (!ViewCollider.contains(P->Body.getPosition()))
		{ //if out of bounds
			P->InUse = false;
			P->FiredFromState = None;
		}


		for (auto B : Boats)
		{
			assert(P);

			if (!B.IsBoatAlive() || B.GetControlState() == P->FiredFromState)
			{
				continue;
			}

			if (P->Body.getGlobalBounds().intersects(B.GetGlobalBounds()))
			{
				P->InUse = false;
				P->FiredFromState = None;
			}
		}

	}


}

void ProjectileManager::draw(sf::RenderTarget & RTarget, sf::RenderStates RStates) const
{

	for (auto P : Projectiles_)
	{
		if (!P->InUse)
		{
			//continue;
		}
		RTarget.draw(P->Body);
	}
}

ProjectileManager::ProjectileType ProjectileManager::BoatTypeToProjectileType(BoatType Type) const
{

	if (Type == Raft || Type == RowingBoat)
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

	for (Int32 i = (Int32)Type * ProjectilePerType_; i < ProjectileCount_; ++i)
	{
		if (Projectiles_[i]->ProjectileType == Type && !Projectiles_[i]->InUse)
		{
			return i;
		}
	}
	return INDEX_NONE;
}
