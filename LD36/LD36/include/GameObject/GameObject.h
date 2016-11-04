#pragma once

#include "MeshCollider\MeshCollider.h"

//TODO Create initialise collider function


class GameObject : public Drawable, public Transformable
{
public:

	GameObject();

	~GameObject();

	void SetTexture(sf::Texture* Tex);

	void SetTextureRect(const IntRect& Rect);

	void SetSize(Vector2f Size);

	Vector2f GetSize() const { return Shape_.getSize(); }

	FloatRect GetAABB() const;

	FloatRect GetLocalBounds() const;

	void SetCollider();

	void SetActive() { IsActive_ = true; }

	void SetInActive() { IsActive_ = false; }

	void SetColour(Colour Col) { Shape_.setFillColor(Col); }

	void ClearColour() { Shape_.setFillColor(Colour::White); }

	bool IsActive() const { return IsActive_; }

	const MeshCollider& GetMeshCollider() const { return MeshCollider_; }

	void UpdateCollider();

private:

	virtual void draw(RenderTarget& RTarget, RenderStates RStates) const;

	RectangleShape Shape_;

	bool IsActive_ = true;

	MeshCollider MeshCollider_;

};

