#pragma once
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include "../Components/AnimationComponent.h"
class AnimationSystem : public System {

public:

	AnimationSystem(Registry* registry) 
		:
		System(registry)
	{
		RequireComponent<AnimationComponent>();
	}

	void Update(float dt) {

		int currentTick = SDL_GetTicks();
		
		for (Entity entity : GetEntities()) {
			AnimationComponent& animData = registry->GetComponent<AnimationComponent>(entity);
			if (currentTick - animData.startTime > animData.frameSpeedRate) {
				
				animData.currentFrame += 1;
				if (animData.currentFrame > animData.numFrames ){
					if (animData.isLoop)
						animData.currentFrame = 1;
					else
						animData.currentFrame = animData.numFrames;
				}
				animData.startTime = currentTick;
			}
		}
	}

};