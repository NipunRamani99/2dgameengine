#pragma once
#include <SDL2/SDL.h>
struct SpriteComponent {
	SDL_Rect srcRect;
	SDL_Texture* texture;
	SpriteComponent() = default;
	SpriteComponent(SDL_Rect srcRect, SDL_Texture * texture) 
		: 
		srcRect(srcRect),
		texture(texture) 
	{

	}
};