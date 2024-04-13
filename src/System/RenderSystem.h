#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL2/SDL.h>
class RenderSystem : public System {

private:
	SDL_Renderer* renderer;

public:
	RenderSystem(Registry * registry, SDL_Renderer * renderer) 
		:
		System(registry),
		renderer(renderer)
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<AnimationComponent>();
		Logger::Log("Initialized RenderSystem");

	}

	void Update(float deltaTime, SDL_Rect & camera) {
		for (int i = 0; i < 3; i++) {
			for (Entity entity : GetEntities()) {

				const TransformComponent& transform = registry->GetComponent<TransformComponent>(entity);
				SpriteComponent& sprite = registry->GetComponent<SpriteComponent>(entity);
				const AnimationComponent& animData = registry->GetComponent<AnimationComponent>(entity);

				if (sprite.zIndex == i) {
					SDL_Texture* texture = sprite.texture;
					
					SDL_Rect& srcRect = sprite.srcRect;

					srcRect.x = 32 * (animData.currentFrame - 1);

					SDL_Rect destRect;
					destRect.x = transform.position.x - camera.x;
					destRect.y = transform.position.y - camera.y;
					destRect.w = transform.scale.x * srcRect.w;
					destRect.h = transform.scale.y * srcRect.h;
					 
					SDL_RenderCopyEx(renderer,
						texture,
						&srcRect,
						&destRect,
						transform.rotation,
						NULL,
						SDL_FLIP_NONE);
				}
			} 
		}
	}

	~RenderSystem() {
		Logger::Log("RenderSystem Destructed.");
	}
};