#pragma once
#include "../ECS/ECS.h"
#include "../Events/KeyboardEvent.h"
#include "../Events/EventBus.h"
#include "../Components/KeyboardMovementComponent.h"
#define MAX_SCANCODES 2048

class KeyboardMovementSystem : public System {
private:

	// Structure to hold key states
	typedef struct {
		int pressed[MAX_SCANCODES];
	} KeyStates;
	KeyStates keyStates;

	enum DIRECTION {
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 3,
		NONE = 4
	};

public:
	KeyboardMovementSystem(Registry* registry, const std::unique_ptr<EventBus>& eventBus)
		:
		System(registry)
	{
		RequireComponent<KeyboardMovementComponent>();
		RequireComponent<SpriteComponent>();
		//RequireComponent<RigidBodyComponent>();
		eventBus->subscribe(this, &KeyboardMovementSystem::KeyboardInputHandler);
		memset(keyStates.pressed, SDL_KEYUP, sizeof(keyStates.pressed));
	}

	void KeyboardInputHandler(KeyboardEvent* keyboardEvent) {
		for (Entity entity : GetEntities()) {

			SpriteComponent& spriteData = registry->GetComponent<SpriteComponent>(entity);
			RigidBodyComponent& rigidBodyData = registry->GetComponent<RigidBodyComponent>(entity);
			KeyboardMovementComponent& kbdMovementData = registry->GetComponent<KeyboardMovementComponent>(entity);
			SDL_KeyboardEvent event = keyboardEvent->sdlKeyEvent;
			Logger::Log("KeyboardEvent " + std::to_string((char)keyboardEvent->sdlKeyEvent.keysym.sym) + " " + std::string((event.type == SDL_KEYDOWN) ? "Pressed" : "Released"));
			DIRECTION direction = NONE;
			if (event.type == SDL_KEYDOWN) {

				// Self explanatory ngl
				// will crash if any other key is pressed lmao
				if (event.keysym.sym < 0) continue;
				switch(event.keysym.sym){
				case SDLK_w:
					direction = UP;
					break;
				case SDLK_d:
					direction = RIGHT;
					break;
				case SDLK_s:
					direction = DOWN;
					break;
				case SDLK_a:
					direction = LEFT;
					break;
				default:
					return;
				}

				// clockwise W -> A -> S -> D to match how the sprite sheet is created
				rigidBodyData.velocity = kbdMovementData.velocities[direction];
				spriteData.srcRect = SDL_Rect{ 0, 32 * direction, 32, 32 };
			}
			else {
				// Set key state to up when SDL_KEYUP event is received
				keyStates.pressed[event.keysym.sym] = SDL_KEYUP;

				// Search for keys which are in KEYDOWN state
				if (keyStates.pressed[SDLK_w] == SDL_KEYDOWN) direction = UP;
				if (keyStates.pressed[SDLK_d] == SDL_KEYDOWN) direction = RIGHT;
				if (keyStates.pressed[SDLK_s] == SDL_KEYDOWN) direction = DOWN;
				if (keyStates.pressed[SDLK_a] == SDL_KEYDOWN) direction = LEFT;

				// If no pressed keys are found we just stop the entity
				if (direction == NONE)
					rigidBodyData.velocity = glm::vec2{ 0.0, 0.0 };
				else {

					// Else use the direction to get the velocities and direction
					rigidBodyData.velocity = kbdMovementData.velocities[direction];
					spriteData.srcRect = SDL_Rect{ 0, 32 * direction, 32, 32 };
				}
			}

		}
	}
};
