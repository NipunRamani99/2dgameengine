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
			SDL_Rect rect;
			rect.w = sprite.width;
			rect.h = sprite.height;
			rect.x = (int)transform.position.x;
			rect.y = (int)transform.position.y;
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
};