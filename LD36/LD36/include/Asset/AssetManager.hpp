#pragma once

#include <unordered_map>
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

	AssetManager();

	Texture* AlreadyHaveTexture(const std::wstring& FilePath);

	std::map<std::wstring, Texture*>Textures_;
	std::map<std::pair<String, String>, Shader*> Shaders_;

	Font DefaultFont_;

	bool HasFontBeenLoaded = false;
};
