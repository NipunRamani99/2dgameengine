#pragma once
#include <SDL2/SDL.h>

struct KeyboardEvent {
	SDL_Keycode keyCode;
	SDL_Scancode scanCode;
	Uint16 modifiers;
	bool isPressed = false;
	bool isRepeated = false;

	KeyboardEvent(SDL_Keycode keyCode, SDL_Scancode scanCode, Uint16 modifiers, bool isPressed, bool isRepeated) 
		:
		keyCode(keyCode),
		scanCode(scanCode),
		modifiers(modifiers),
		isPressed(isPressed),
		isRepeated(isRepeated)
	{
	}
};