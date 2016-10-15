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
	Data.Filepath = FilePath.toWideString();
	Data.Texture = new sf::Texture;
	if (Data.Texture == nullptr)
	{
		DebugPrintF(AssetLog, L"Failed to load texture at %s", Data.Filepath);
		return nullptr;
	}
	if (!Data.Texture->loadFromFile(FilePath))
	{
		DebugPrintF(AssetLog, L"Failed to load texture at %s", Data.Filepath);
		return nullptr;
	}

	Textures_.emplace(Data.Filepath, nullptr);
	Textures_[Data.Filepath] = Data.Texture;
	return Textures_[Data.Filepath];
}

AssetManager::AssetManager()
{
}

sf::Texture* AssetManager::AlreadyHaveTexture(const std::wstring & FilePath)
{
	return Textures_[FilePath];
}
