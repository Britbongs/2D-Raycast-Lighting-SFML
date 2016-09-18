#pragma once

class AssetManager
{
public:
	~AssetManager()
	{
		for (TextureData& D : Textures_)
		{
			delete D.Texture;
			D.Texture = nullptr;
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
		sf::String Filepath = "";
		sf::Texture* Texture = nullptr;
	};

	AssetManager();

	sf::Texture* AlreadyHaveTexture(const sf::String& FilePath);

	std::vector<TextureData> Textures_;

	sf::Font DefaultFont_;
	bool HasFontBeenLoaded = false;
};

