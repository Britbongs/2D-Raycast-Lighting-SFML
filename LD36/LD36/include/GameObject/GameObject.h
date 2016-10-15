#pragma once

#include "MeshCollider\MeshCollider.h"

//TODO Create initialise collider function

enum ObjectType;

class GameObject : public Drawable, public Transformable
{
public:

	GameObject(ObjectType);

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

	void SetObjectType(ObjectType Type) { Type_ = Type; }

	bool IsActive() const { return IsActive_; }

	ObjectType GetObjectType() const { return Type_; }

	const MeshCollider& GetMeshCollider() const { return MeshCollider_; }

private:

	virtual void draw(RenderTarget& RTarget, RenderStates RStates) const;

	RectangleShape Shape_;

	bool IsActive_ = false;

	ObjectType Type_;

	MeshCollider MeshCollider_;

};

