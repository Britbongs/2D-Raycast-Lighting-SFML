#include "stdafx.hpp"
#include "Asset\AssetManager.hpp"


AssetManager::~AssetManager()
{
	for (auto& TexMapPair : Textures_)
	{
		delete TexMapPair.second;
		TexMapPair.second = nullptr;
	}

	Textures_.clear();

	for (auto& ShaderMapPair : Shaders_)
	{
		delete ShaderMapPair.second;
		ShaderMapPair.second = nullptr;
	}

	Shaders_.clear();
}

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
	if (Textures_.find(FilePath) == Textures_.end())
	{
		Textures_.emplace(FilePath, new Texture());
		if (!Textures_[FilePath]->loadFromFile(FilePath))
		{
			delete Textures_[FilePath];
			Textures_[FilePath] = nullptr;
			Textures_.erase(FilePath);
			DebugPrintF(ErrorLog, L"Failed to load texture at path: %s !", FilePath.toWideString());
			return nullptr;
		}
	}
	return Textures_[FilePath];
}

Shader* AssetManager::LoadShader(const String& VertFilepath, const String& FragFilepath)
{
	std::pair<String, String> Key(VertFilepath, FragFilepath);
	//If no shader exists in the map with the two file paths passed 
	if (Shaders_.find(Key) == Shaders_.end())
	{
		Shaders_.emplace(Key, new Shader());

		if (!Shaders_[Key]->loadFromFile(Key.first, Key.second))
		{ //Failed to load the shader, free the memory and return a nullptr 
			delete Shaders_[Key];
			Shaders_[Key] = nullptr;
			Shaders_.erase(Key);
			return nullptr;
		}
	}

	return Shaders_[Key];
}

AssetManager::AssetManager()
{
}

sf::Texture* AssetManager::AlreadyHaveTexture(const std::wstring & FilePath)
{
	return Textures_[FilePath];
}
