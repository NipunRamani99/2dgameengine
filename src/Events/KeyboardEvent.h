#pragma once
#include <SDL2/SDL.h>

struct KeyboardEvent {
	SDL_KeyboardEvent sdlKeyEvent;

	KeyboardEvent(SDL_KeyboardEvent sdlKeyEvent) 
		:
		sdlKeyEvent(sdlKeyEvent)
	{
	}
};