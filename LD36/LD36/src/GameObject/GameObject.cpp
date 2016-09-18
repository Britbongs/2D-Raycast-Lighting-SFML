#include "stdafx.h"
#include "GameObject\GameObject.h"
#include "World\World.h"

GameObject::GameObject(ObjectType Type = ObjectType::eEmpty)
	:Type_(Type)
{
	Shape_.setSize(Vector2f(1.f, 1.f));
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

void GameObject::RefreshColliderData()
{

}
