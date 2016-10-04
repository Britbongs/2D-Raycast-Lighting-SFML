#include "stdafx.h"
#include "Projectile\ProjectileManager.hpp"
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

}

ProjectileManager::~ProjectileManager()
{
	delete[] ProjectileSpeeds_;
	ProjectileSpeeds_ = nullptr;
}

bool ProjectileManager::SetupProjectiles()
{
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
}

void ProjectileManager::ProjectileUpdate(float Delta)
{
	for (auto P : Projectiles_)
	{
		if (P->IsActive() == false)
		{
			continue;
		}

		P->move(Vector2f());

		printf("%f - %f\n", P->getPosition().x, P->getPosition().y);

		if (!World_->IsInsideView(P->GetAABB()))
		{ //if out of bounds
			P->SetInActive();
			continue;
		}
		//Collision query

	}
}
