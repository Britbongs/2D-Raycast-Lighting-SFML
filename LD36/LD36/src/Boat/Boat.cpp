#include "stdafx.h"
#include "Boat\Boat.hpp"
#include "DerivedStates\PlayState.hpp"

using namespace sf;

Boat::Boat(ProjectileManager* Projectiles, BoatControlState ControlState, BoatType Type)
	: Projectile_(Projectiles), ControlState_(ControlState), BoatType_(Type)
{
	Shape_.setSize(BoatSize);
	ColShape_.setSize(BoatSize);
	ColShape_.setFillColor(sf::Color::Green);
}

Boat::~Boat()
{

}

void Boat::SetTexture(sf::Texture * BoatTex, sf::Texture* BuoyTex)
{
	if (BoatTex != nullptr && BuoyTex != nullptr)
	{
		BoatTex_ = BoatTex;
		Shape_.setTexture(BoatTex);

		BuoyTex_ = BuoyTex;
	}
}

void Boat::SetTextureRect(const IntRect & TexRect)
{
	Shape_.setTextureRect(TexRect);
}

void Boat::SetColour(Color Colour)
{
	Shape_.setFillColor(Colour);
}

sf::FloatRect Boat::GetGlobalBounds() const
{
	FloatRect R(getTransform().transformRect(Shape_.getGlobalBounds()));
	const Vector2f OriginalCentre(R.left + (R.width / 2.f), (R.top + R.height / 2.f));
	float WPercent = 0.8f;
	float HPercent = 0.8f;

	if (getRotation() != 0.f)
	{
		WPercent = 0.40f;
		HPercent = 0.42f;
	}

	R.width = R.width * WPercent;
	R.height = R.height * HPercent;

	R.left = OriginalCentre.x - (R.width / 2.f);
	R.top = OriginalCentre.y - (R.height / 2.f);

	return R;
}

sf::Vector2f Boat::GetSize() const
{
	FloatRect R = GetGlobalBounds();
	return sf::Vector2f(R.width, R.height);
}

void Boat::Fire(Vector2f Direction)
{
	if (ControlState_ == BoatControlState::None || !CanFire_)
	{
		return;
	}

	CanFire_ = false;
}

void Boat::draw(sf::RenderTarget & RTarget, sf::RenderStates RStates) const
{
	RStates.transform *= getTransform();
	RTarget.draw(Shape_, RStates);
}

void Boat::Update(float Delta)
{
	if (ControlState_ == BoatControlState::Player)
	{
		if (!CanFire_ && FireCoolDown_ < 0.45f)
		{
			FireCoolDown_ += Delta;
		}
		else
		{
			FireCoolDown_ = 0.f;
			CanFire_ = true;
		}

		if (!CanTakeDamage_)
		{
			DamageGrace_ += Delta;
			if (DamageGrace_ >= 0.35f)
			{
				CanTakeDamage_ = true;
				DamageGrace_ = 0.f;
			}
		}

		ColShape_.setPosition(GetGlobalBounds().left, GetGlobalBounds().top);
		ColShape_.setSize(Vector2f(GetGlobalBounds().width, GetGlobalBounds().height));

		return;
	}
	//TODO AI code

	if (ControlState_ == AI && !IsAlive_)
	{
		if (!CanFire_ && FireCoolDown_ < 1.25f)
		{
			FireCoolDown_ += Delta;
		}
		else
		{
			float Inaccuracy(RandFloat(0.f, 0.012f));
			Vector2f Direc;//Normalise(Player.getPosition() - getPosition());
			Direc = RotateVector(Direc, Inaccuracy);
			FireCoolDown_ = 0.f;
			CanFire_ = true;
			Fire(Direc);
		}

	}
}
