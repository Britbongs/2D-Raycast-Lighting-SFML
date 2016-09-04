#pragma once

class TiledBackground :
	public sf::Drawable, public sf::Transformable
{
public:

	TiledBackground(sf::Uint32 TileSize, sf::Vector2u Dimensions);

	~TiledBackground();

	void SetTexture(sf::Texture* Tex);

	sf::Vector2f GetSize() const;

	sf::FloatRect GetGlobalBounds() const;

private:

	virtual void draw(sf::RenderTarget& RTarget, sf::RenderStates RStates) const;

	sf::FloatRect GetLocalBounds() const;

	void SetupVetices();

	void SetTextureCoord(sf::Vertex* Tile);

	sf::VertexArray Map_;
	sf::Texture* Texture_ = nullptr;

	const sf::Uint32 TileSize_;
	const sf::Vector2u Dimensions_;
};

