#pragma once
/*
	TODO Convert vector of textures to map
*/
class AssetManager
{
public:
	~AssetManager();

	static AssetManager* GetInstance()
	{
		static AssetManager* Instance = new AssetManager;
		return Instance;
	}

	Font* GetDefaultFont();

	Texture* LoadTexture(const String& FilePath);

	Shader* LoadShader(const String& VertFilepath, const String& FragFilepath);

private:

	struct TextureData
	{
		String Filepath = "";
		Texture* Texture = nullptr;
	};

	AssetManager();

	Texture* AlreadyHaveTexture(const sf::String& FilePath);

	std::vector<TextureData> Textures_;

	std::map<std::pair<String, String>, Shader*> Shaders_;
	//std::map<String, Texture*> TexturesMap_;

	Font DefaultFont_;

	bool HasFontBeenLoaded = false;
};
