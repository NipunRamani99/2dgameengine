#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"
#include <string>
class MovementSystem : public System {
public:
	MovementSystem(Registry * registry) 
		:
		System(registry)
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		Logger::Log("Initialized MovementSystem");

		/**
		* eventBus.subscribeEvent<CollisionEvent>(this, &MovementSystem::ResolveCollision);
		* 
		*/
	}

	void Update(float deltaTime) {
		//Loop all entities that the system is interested in
		for (auto entity : GetEntities()) {
			auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
			const auto & rigidBodyComponent = registry->GetComponent<RigidBodyComponent>(entity);
			transformComponent.position += deltaTime * rigidBodyComponent.velocity;
		}
	}

	void resolveCollision(CollisionEvent event) {
		Logger::Log("Processing Collision For Entities: " + std::to_string(event.a.GetId()) + ", " + std::to_string(event.b.GetId()));
	}

	~MovementSystem() {
		Logger::Log("MovementSystem destructed.");
	}
};