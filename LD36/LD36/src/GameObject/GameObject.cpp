#include "stdafx.hpp"
#include "GameObject\GameObject.h"
#include "World\World.hpp"

GameObject::GameObject()
{
	Shape_.setSize(Vector2f(1.f, 1.f));

	std::vector<Vector2f> PointsList(4);
	PointsList[0] = Vector2f(0.f, 0.f);
	PointsList[1] = Vector2f(1.f, 0.f);
	PointsList[2] = Vector2f(1.f, 1.f);
	PointsList[3] = Vector2f(0.f, 1.f);

	MeshCollider_ = MeshCollider(PointsList);
}

GameObject::~GameObject()
{

}

void GameObject::SetTexture(sf::Texture * Tex)
{
	Shape_.setTexture(Tex, true);
	Shape_.setSize(Vector2f(Tex->getSize()));
}

void GameObject::SetTextureRect(const IntRect & Rect)
{
	Shape_.setTextureRect(Rect);
}

void GameObject::SetSize(Vector2f Size)
{
	Shape_.setSize(Size);
	std::vector<Vector2f> PointsList(4);
	PointsList[0] = Vector2f(0.f, 0.f);
	PointsList[1] = Vector2f(Shape_.getSize().x, 0.f);
	PointsList[2] = Vector2f(Shape_.getSize().x, Shape_.getSize().y);
	PointsList[3] = Vector2f(0.f, Shape_.getSize().y);

}

FloatRect GameObject::GetAABB() const
{
	return getTransform().transformRect(GetLocalBounds());
}

FloatRect GameObject::GetLocalBounds() const
{
	return Shape_.getLocalBounds();
}

void GameObject::SetCollider()
{
}

void GameObject::draw(RenderTarget & RTarget, RenderStates RStates) const
{
	RStates.transform *= getTransform();
	RTarget.draw(Shape_, RStates);
}

