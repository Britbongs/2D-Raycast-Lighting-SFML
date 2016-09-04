#include "stdafx.h"
#include "TiledBackground\TiledBackground.hpp"

using namespace sf;

TiledBackground::TiledBackground(Uint32 TileSize, Vector2u Dimension)
	: TileSize_(TileSize), Dimensions_(Dimension)
{
	SetupVetices();
}

TiledBackground::~TiledBackground()
{
}

void TiledBackground::SetTexture(sf::Texture * Tex)
{
	if (Tex == nullptr)
	{
		return;
	}
	Texture_ = Tex;

	for (Uint32 i = 0; i < Dimensions_.x; ++i)
	{
		for (Uint32 j = 0; j < Dimensions_.y; ++j)
		{
			sf::Vertex* Tile = &Map_[(i + j * Dimensions_.x) * 4];
			SetTextureCoord(Tile);
		}
	}
}

sf::Vector2f TiledBackground::GetSize() const
{
	Vector2f Size;

	Size.x = (float)(Dimensions_.x * TileSize_);
	Size.y = (float)(Dimensions_.y * TileSize_);

	return sf::Vector2f(Size);
}

sf::FloatRect TiledBackground::GetGlobalBounds() const
{
	return getTransform().transformRect(GetLocalBounds());
}

// private

void TiledBackground::draw(sf::RenderTarget & RTarget, sf::RenderStates RStates) const
{
	RStates.transform *= getTransform();

	if (Texture_ != nullptr)
	{
		RStates.texture = Texture_;
	}

	RTarget.draw(Map_, RStates);
}

sf::FloatRect TiledBackground::GetLocalBounds() const
{
	FloatRect R;

	R.left = 0.f;
	R.top = 0.f;
	R.width = GetSize().x;
	R.height = GetSize().y;

	return R;
}

void TiledBackground::SetupVetices()
{
	Map_.setPrimitiveType(sf::Quads);
	Map_.resize(Dimensions_.x * Dimensions_.y * 4);

	for (Int32 i = 0; i < (signed)Dimensions_.x; ++i)
	{
		for (Int32 j = 0; j < (signed)Dimensions_.y; ++j)
		{
			sf::Vertex* Tile = &Map_[(i + j * Dimensions_.x) * 4];

			Tile[0].position = Vector2f((float)i* TileSize_, j * TileSize_);
			Tile[1].position = Vector2f((i + 1) * TileSize_, j * TileSize_);
			Tile[2].position = Vector2f((i + 1) * TileSize_, (j + 1) * TileSize_);
			Tile[3].position = Vector2f(i * TileSize_, (j + 1)*TileSize_);
		}
	}
}

void TiledBackground::SetTextureCoord(Vertex* Tile)
{
	assert(Texture_);

	Vector2f TexSize(Texture_->getSize());
	Int32 TexId(rand() % 100);
	TexId > 50 ? TexId = 0 : TexId = 1;

	Tile[0].texCoords = Vector2f(TexId * TileSize_, 0.f);
	Tile[1].texCoords = Vector2f((TexId + 1)* TileSize_, 0.f);
	Tile[2].texCoords = Vector2f((TexId + 1) * TileSize_, TileSize_);
	Tile[3].texCoords = Vector2f(TexId * TileSize_, TileSize_);
}
