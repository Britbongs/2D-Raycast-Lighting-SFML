#pragma once

struct ColliderData
{
	//TODO sat collider info 
};

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

	const ColliderData& GetColliderData()
	{
		RefreshColliderData();
		return Collider_;
	};

	void SetCollider();

	void SetActive() { IsActive_ = true; }

	void SetInActive() { IsActive_ = false; }
	
	void SetObjectType(ObjectType Type) { Type_ = Type; }

	bool IsActive() const { return IsActive_; }

	ObjectType GetObjectType() const { return Type_; }

private:

	virtual void draw(RenderTarget& RTarget, RenderStates RStates) const;

	void RefreshColliderData();

	RectangleShape Shape_;

	ColliderData Collider_;

	bool IsActive_ = false;

	ObjectType Type_;

};

