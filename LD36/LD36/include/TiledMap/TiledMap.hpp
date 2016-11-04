#pragma once

#include "TMXLoader\TMXLoader.hpp"

class TiledMap :
	public sf::Drawable, public sf::Transformable
{
public:

	TiledMap(sf::Uint32 TileSize);

	~TiledMap();

	void SetTexture(sf::Texture* Tex);

	Vector2f GetSize() const;

	Vector2i GetGridSize() const;

	Int32 GetTileSize() const;

	sf::FloatRect GetGlobalBounds() const;

	bool GetCollideableAtIndex(Int32 index) const;

	void SetupVetices(const TMXLoader& Loader);

private:

	virtual void draw(sf::RenderTarget& RTarget, sf::RenderStates RStates) const;

	sf::FloatRect GetLocalBounds() const;


	void SetTextureCoord(sf::Vertex* Tile, Int32 TileID);

	sf::VertexArray Map_;

	std::vector<bool> CollisionMap_;

	Int32 TileIDs_[16][12] =
	{
		{ 1,1,1,1,1,1,1,1,1,1,1,1 },
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

	const Int32 TileSize_;
	Vector2i Dimensions_;
};

