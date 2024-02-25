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
		Logger::Log("Initialized RenderSystem");

	}

	void Update(float deltaTime) {
		for (Entity entity : GetEntities()) {
			const TransformComponent & transform = registry->GetComponent<TransformComponent>(entity);
			const SpriteComponent& sprite = registry->GetComponent<SpriteComponent>(entity);
			SDL_Texture* texture = sprite.texture;
			const SDL_Rect& srcRect = sprite.srcRect;
			SDL_Rect destRect;
			destRect.x = transform.position.x;
			destRect.y = transform.position.y;
			destRect.w = transform.scale.x * srcRect.w;
			destRect.h = transform.scale.y * srcRect.h;
		
			SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		}
	}
};