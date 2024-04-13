#pragma once
#include "../ECS/ECS.h"
#include "../Components/TileComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
class TileMapRenderSystem : public System {
private:
	SDL_Renderer* renderer;
	std::vector<std::vector<size_t>> tileMap;
public:
	TileMapRenderSystem(Registry* registry, SDL_Renderer* renderer) 
		:
		System(registry),
		renderer(renderer)
	{
		RequireComponent<TileComponent>();
		RequireComponent<SpriteComponent>();
		Logger::Log("Initialized TileMapRenderSystem");

	}

	void LoadTileMap() {
		std::ifstream inputFile("./assets/tilemaps/jungle.map");
		// Check if the file is open
		if (!inputFile.is_open()) {
			Logger::Err("Error opening the file.");
			return;
		}
		std::string line;
		while (std::getline(inputFile, line)) {
			std::istringstream iss(line);
			std::vector<size_t> row;
			
			int tileType;
			while (iss >> tileType) {
				row.push_back(tileType);
				if (iss.peek() == ',') {
					iss.ignore();
				}
			}
			tileMap.push_back(row);
		}
		inputFile.close();
	}

	std::tuple<int, int> GetMapDim(float tileScale, int tileSize) {
		int mapHeight = tileScale * tileSize * tileMap.size();
		int mapWidth = tileScale * tileSize * tileMap[0].size();
		return { mapWidth, mapHeight };
	}

	void Update(float dt, SDL_Rect & camera) {
		std::vector<Entity> tiles;
		tiles.resize(30);
		for (Entity entity : GetEntities()) {
			const TileComponent & tileMapData = registry->GetComponent<TileComponent>(entity);
			tiles[tileMapData.idx] = entity;
		}

		// 2d loop over the scene
		for (size_t y = 0; y < tileMap.size(); y++) {
			for (size_t x = 0; x < tileMap[y].size(); x++) {
				size_t idx = tileMap[y][x];
				Entity tile = tiles[idx];
				const SpriteComponent & spriteComponent = registry->GetComponent<SpriteComponent>(tile);
				const TileComponent& tileMapData = registry->GetComponent<TileComponent>(tile);

				SDL_Texture* texture = spriteComponent.texture;
				const SDL_Rect& srcRect = spriteComponent.srcRect;
				SDL_Rect destRect;
				destRect.x = x*32 * tileMapData.scale.x - camera.x;
				destRect.y = y*32 * tileMapData.scale.y - camera.y;
				destRect.w = srcRect.w * tileMapData.scale.x;
				destRect.h = srcRect.h * tileMapData.scale.y;

				SDL_RenderCopyEx(
					renderer,
					texture,
					&srcRect,
					&destRect,
					0,
					NULL,
					SDL_FLIP_NONE
				);
			}
		}
	}
};