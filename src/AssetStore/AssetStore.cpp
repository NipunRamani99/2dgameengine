#include "AssetStore.h"
#include <SDL2/SDL_Image.h>

void AssetStore::ClearAssets()
{
	for (auto [_, texture] : textureStore) {
		SDL_DestroyTexture(texture);
	}

	textureStore.clear();
}

void AssetStore::AddTexture(const std::string& assetId, const std::string& filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (!surface) {
		Logger::Err("Could not load texture: " + assetId + " from file: " + filePath);
	} else {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		textureStore.insert(std::make_pair(assetId, texture));
		Logger::Log("Texture " + assetId + " loaded successfully from file: " + filePath);
	}
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)
{
	return textureStore[assetId];
}
