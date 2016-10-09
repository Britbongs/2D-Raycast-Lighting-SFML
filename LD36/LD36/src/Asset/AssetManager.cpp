#include "stdafx.hpp"
#include "Asset\AssetManager.hpp"


sf::Font* AssetManager::GetDefaultFont()
{
	if (!HasFontBeenLoaded)
	{
		DefaultFont_.loadFromFile("res//fonts//tinnies.ttf");
		HasFontBeenLoaded = true;
	}
	return &DefaultFont_;
}

sf::Texture * AssetManager::LoadTexture(const sf::String & FilePath)
{
	sf::Texture* Tex = AlreadyHaveTexture(FilePath);

	if (Tex != nullptr)
	{
		return Tex;
	}

	TextureData Data;
	Data.Filepath = FilePath;
	Data.Texture = new sf::Texture;
	if (Data.Texture == nullptr)
	{
#if !PLAYABLE_BUILD
		DebugPrintF(AssetLog, L"Failed to load texture at %s", FilePath.toWideString());
#endif
		return nullptr;
	}
	if (!Data.Texture->loadFromFile(FilePath))
	{
#ifndef PLAYABLE_BUILD
		DebugPrintF(AssetLog, L"Failed to load texture at %s", FilePath.toWideString());
#endif
		return nullptr;
	}
	Textures_.push_back(Data);
	return Textures_.back().Texture;
}

AssetManager::AssetManager()
{
}

sf::Texture* AssetManager::AlreadyHaveTexture(const sf::String & FilePath)
{
	for (TextureData& Data : Textures_)
	{
		if (Data.Filepath == FilePath)
		{
			return Data.Texture;
		}
	}
	return nullptr;
}
