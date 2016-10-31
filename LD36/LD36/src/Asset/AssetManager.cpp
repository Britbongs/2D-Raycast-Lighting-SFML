#include "stdafx.hpp"
#include "Asset\AssetManager.hpp"


AssetManager::~AssetManager()
{
	for (TextureData& D : Textures_)
	{
		delete D.Texture;
		D.Texture = nullptr;
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
		DebugPrintF(AssetLog, L"Failed to load texture at %s", FilePath.toWideString());
		return nullptr;
	}
	Textures_.push_back(Data);
	return Textures_.back().Texture;
}

Shader* AssetManager::LoadShader(const String& VertFilepath, const String& FragFilepath)
{
	std::pair<String, String> Key(FragFilepath, VertFilepath);
	//If no shader exists in the map with the two file paths passed 
	if (Shaders_.find(Key) == Shaders_.end())
	{
		Shaders_.emplace(Key, new Shader());

		if (!Shaders_[Key]->loadFromFile(Key.first, Key.second))
		{ //Failed to load the shader, free the memory and return a nullptr 
			delete Shaders_[Key];
			Shaders_[Key] = nullptr;
			Shaders_.erase(Key);
			DebugPrintF(ErrorLog, L"Failed to load shader: Vert =  %s : Frag = %s!", VertFilepath.toWideString(), FragFilepath.toWideString());
			return nullptr;
		}
	}

	return Shaders_[Key];
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
