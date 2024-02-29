#pragma once
#include <SDL2/SDL.h>
struct AnimationComponent {
	int currentFrame = 1;
	int numFrames = 1;
	int frameSpeedRate;
	bool isLoop;
	int startTime;

	AnimationComponent() {
	}

	AnimationComponent(int numFrames, int frameSpeedRate, bool isLoop = true) 
		:
		currentFrame(1),
		numFrames(numFrames),
		frameSpeedRate(frameSpeedRate),
		isLoop(isLoop),
		startTime(SDL_GetTicks())
	{
		
	}
};