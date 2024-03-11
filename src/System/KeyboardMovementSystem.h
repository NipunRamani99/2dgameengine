#pragma once
#include "../ECS/ECS.h"
#include "../Events/KeyboardEvent.h"
#include "../Events/EventBus.h"
#include "../Components/KeyboardMovementComponent.h"
class KeyboardMovementSystem : public System{
private:

public:
	KeyboardMovementSystem(Registry * registry, const std::unique_ptr<EventBus> & eventBus) 
		:
		System(registry)
	{
		RequireComponent<KeyboardMovementComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
		eventBus->subscribe(this, &KeyboardMovementSystem::KeyboardInputHandler);
	}

	void KeyboardInputHandler(KeyboardEvent * keyboardEvent) {
		for (Entity entity : GetEntities()) {
			
			SpriteComponent& spriteData = registry->GetComponent<SpriteComponent>(entity);
			RigidBodyComponent& rigidBodyData = registry->GetComponent<RigidBodyComponent>(entity);
			KeyboardMovementComponent& kbdMovementData = registry->GetComponent<KeyboardMovementComponent>(entity);

			if (keyboardEvent->isPressed) {
				switch (keyboardEvent->keyCode) {
				case SDLK_w:
					rigidBodyData.velocity = kbdMovementData.upVelocity;
					spriteData.srcRect = SDL_Rect{ 0,0, 32, 32 };
					break;
				case SDLK_d:
					rigidBodyData.velocity = kbdMovementData.rightVelocity;
					spriteData.srcRect = SDL_Rect{ 0,32, 32, 32 };
					break;
					break;
				case SDLK_s:
					rigidBodyData.velocity = kbdMovementData.downVelocity;
					spriteData.srcRect = SDL_Rect{ 0, 64, 32, 32 };
					break;
					break;
				case SDLK_a:
					rigidBodyData.velocity = kbdMovementData.leftVelocity;
					spriteData.srcRect = SDL_Rect{ 0, 96, 32, 32 };
					break;
				}
			}
			else {
				rigidBodyData.velocity = glm::vec2{ 0.0f,0.0f };
			}
		}
	}
};
