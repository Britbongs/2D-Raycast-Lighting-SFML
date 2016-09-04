#pragma once
#include "Projectile\ProjectileManager.hpp"	
#define BOAT_TYPE_COUNT 5

enum BoatControlState
{
	None,
	Player,
	AI
};

enum BoatType
{
	Raft,
	RowingBoat,
	SailBoat,
	SteamBoat,
	Warship
};


//Forward declare
struct Projectile;

class Boat : public sf::Drawable, public sf::Transformable
{

public:

	Boat(ProjectileManager* Projectiles, BoatControlState ControlState = BoatControlState::None, BoatType Type = BoatType::Raft);

	~Boat();

	void Update(float Delta);

	void SetTexture(sf::Texture* BoatTex, sf::Texture* BuoyTex);

	void SetTextureRect(const sf::IntRect& TexRect);

	void SetColour(sf::Color Colour = Color::White);

	sf::FloatRect GetGlobalBounds() const;

	sf::Vector2f GetSize() const;

	BoatControlState GetControlState() const { return ControlState_; }

	BoatType GetBoatType() const { return BoatType_; }

	void Fire(Vector2f Direction);

	void SetControlState(BoatControlState State) { ControlState_ = State; }

	sf::Int32 GetHealthPoints() const { return HealthPoints_; }

	void LoseHealthPoints(sf::Int32 DeltaHP) { HealthPoints_ -= DeltaHP; }

	void SetHealthPoints(sf::Int32 HP) { HealthPoints_ = HP; }

	void SetSize(sf::Vector2f Size) { BoatSize = Size; Shape_.setSize(Size); ColShape_.setSize(Size); }

	void SetBoatDead() { Shape_.setTexture(BuoyTex_); IsAlive_ = false; }

	void SetBoatAlive() { Shape_.setTexture(BoatTex_); IsAlive_ = true; }

	bool IsBoatAlive() const { return IsAlive_; }

	void ResetFireTimer()
	{
		CanFire_ = true;
		FireCoolDown_ = 0.f;
	}

	bool CanTakeDamage() const { return CanTakeDamage_; }

	void TakenDamage() { CanTakeDamage_ = false; }

private:

	virtual void draw(sf::RenderTarget& RTarget, sf::RenderStates RStates) const;

	BoatControlState ControlState_ = BoatControlState::None;

	BoatType BoatType_ = BoatType::Raft;

	sf::RectangleShape Shape_;
	sf::RectangleShape ColShape_;

	sf::Texture* BoatTex_ = nullptr;
	sf::Texture* BuoyTex_ = nullptr;

	sf::Int32 HealthPoints_ = 100;

	sf::Vector2f BoatSize = sf::Vector2f(64.f, 64.f);

	ProjectileManager* Projectile_ = nullptr;

	float FireCoolDown_ = 0.f;
	float DamageGrace_ = 0.f;

	bool CanFire_ = true;
	bool IsAlive_ = false;
	bool CanTakeDamage_ = true;

};

