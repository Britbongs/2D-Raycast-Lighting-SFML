#pragma once

#include <unordered_map>

class AssetManager
{
public:
	~AssetManager()
	{
		for (auto & D : Textures_)
		{
			delete D.second;
			D.second= nullptr;
		}
	}

	static AssetManager* GetInstance()
	{
		static AssetManager* Instance = new AssetManager;
		return Instance;
	}

	sf::Font* GetDefaultFont();

	sf::Texture* LoadTexture(const sf::String& FilePath);

private:

	struct TextureData
	{
		std::wstring Filepath = L"";
		Texture* Texture = nullptr;
	};

	AssetManager();

	sf::Texture* AlreadyHaveTexture(const std::wstring& FilePath);

	//std::vector<TextureData> Textures_;
	std::unordered_map<std::wstring, Texture*>Textures_;

	sf::Font DefaultFont_;
	bool HasFontBeenLoaded = false;
};
