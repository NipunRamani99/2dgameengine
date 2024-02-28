#pragma once
#include <SDL2/SDL.h>
struct SpriteComponent {
	SDL_Rect srcRect;
	SDL_Texture* texture;
	int zIndex = -1;
	SpriteComponent() = default;
	SpriteComponent(SDL_Rect srcRect, SDL_Texture * texture, int zIndex) 
		: 
		srcRect(srcRect),
		texture(texture),
		zIndex(zIndex)
	{

	}
};