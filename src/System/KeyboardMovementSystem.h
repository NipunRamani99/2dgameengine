#pragma once
#include "../ECS/ECS.h"
#include "../Events/KeyboardEvent.h"
#include "../Events/EventBus.h"

class KeyboardMovementSystem : public System{
private:

public:
	KeyboardMovementSystem(Registry * registry, const std::unique_ptr<EventBus> & eventBus) 
		:
		System(registry)
	{
		eventBus->subscribe(this, &KeyboardMovementSystem::KeyboardInputHandler);
	}

	void KeyboardInputHandler(KeyboardEvent* keyboardEvent) {
		Logger::Log("Keyboard Event Received");
	}

};
