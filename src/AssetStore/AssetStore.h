#pragma once
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "../Logger/Logger.h"

class AssetStore {
private:
	std::map<std::string, SDL_Texture*> textureStore;
	//TODO: std::map<std::string, SDL_Audio*> audioStore;
	//TODO: std::map<std::string, SDL_Font*> fontStore;

	SDL_Renderer* renderer = nullptr;
public:
	AssetStore(SDL_Renderer * renderer) 
		:
		renderer(renderer)
	{
		Logger::Log("AssetStore constructed.");
		
	}
	~AssetStore() {
		ClearAssets();
		Logger::Log("AssetStore destructed.");
	}
	void ClearAssets();
	void AddTexture(const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string& assetId);
};