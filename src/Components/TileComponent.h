#pragma once
#include <string>

struct TileComponent {
	std::string tileMap;
	size_t idx = 0;
	glm::vec2 scale = {1.0f,1.0f};
	TileComponent() {
		tileMap = "";
		idx = 0;
	}

	TileComponent(const std::string tileMap, size_t idx, glm::vec2 scale) 
		:
		tileMap(tileMap),
		idx(idx),
		scale(scale)
	{
	}
};