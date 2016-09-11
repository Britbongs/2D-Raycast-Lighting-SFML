#pragma once

struct ColliderData
{
	//TODO sat collider info 
};

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

	ColliderData GetColliderData() const { return Collider_; };

	void SetCollider();

	void SetActive() { IsActive_ = true; }

	void SetInActive() { IsActive_ = false; }

	bool IsActive() const { return IsActive_; }

private:

	virtual void draw(RenderTarget& RTarget, RenderStates RStates) const;

	RectangleShape Shape_;

	ColliderData Collider_;

	bool IsActive_ = false;

};

