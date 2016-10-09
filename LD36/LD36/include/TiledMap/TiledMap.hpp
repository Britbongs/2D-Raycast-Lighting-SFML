#pragma once

class TiledMap :
	public sf::Drawable, public sf::Transformable
{
public:

	TiledMap(sf::Uint32 TileSize, sf::Vector2u Dimensions);

	~TiledMap();

	void SetTexture(sf::Texture* Tex);

	sf::Vector2f GetSize() const;

	sf::FloatRect GetGlobalBounds() const;

private:

	virtual void draw(sf::RenderTarget& RTarget, sf::RenderStates RStates) const;

	sf::FloatRect GetLocalBounds() const;

	void SetupVetices();

	void SetTextureCoord(sf::Vertex* Tile, Int32 TileID);

	sf::VertexArray Map_;

	std::vector<Int32> CollisionMap_;

	Int32 TileIDs_[16][12] =
	{
		{ 270,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1 }
	};

	sf::Texture* Texture_ = nullptr;

	const sf::Uint32 TileSize_;
	const sf::Vector2u Dimensions_;
};

