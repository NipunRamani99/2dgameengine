#pragma once
#include "../Utils/Constants.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"
#include <string>
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"
class MovementSystem : public System {
public:
	MovementSystem(Registry * registry, EventBus * eventBus) 
		:
		System(registry)
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		Logger::Log("Initialized MovementSystem");

	    eventBus->subscribe(this, &MovementSystem::resolveCollision);
		
	}

	void Update(float deltaTime) {
		//Loop all entities that the system is interested in
		for (auto entity : GetEntities()) {
			auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
			auto & rigidBodyComponent = registry->GetComponent<RigidBodyComponent>(entity);
			transformComponent.position += deltaTime * rigidBodyComponent.velocity;
			auto& dim = rigidBodyComponent.dim;
			if (transformComponent.position.x < 0 || transformComponent.position.x + dim.x*transformComponent.scale.x - 1 > SCREEN_WIDTH) {

				rigidBodyComponent.velocity.x *= -1.0f;
			}
			if (transformComponent.position.y < 0 || transformComponent.position.y + dim.y*transformComponent.scale.y - 1 > SCREEN_HEIGHT) {
				rigidBodyComponent.velocity.y *= -1.0f;
			}
		}
	}

	void resolveCollision(CollisionEvent * event) {
		Logger::Log("Processing Collision For Entities: " + std::to_string(event->a.GetId()) + ", " + std::to_string(event->b.GetId()));
		Entity a = event->a;
		Entity b = event->b;
		auto& transformComponentA = registry->GetComponent<TransformComponent>(a);
	    auto & rigidBodyComponentA = registry->GetComponent<RigidBodyComponent>(a);
		auto& transformComponentB = registry->GetComponent<TransformComponent>(b);
	    auto & rigidBodyComponentB = registry->GetComponent<RigidBodyComponent>(b);
		auto rel = rigidBodyComponentB.velocity - rigidBodyComponentA.velocity;
		rigidBodyComponentA.velocity += rel;
		rigidBodyComponentB.velocity -= rel;
	}

	~MovementSystem() {
		Logger::Log("MovementSystem destructed.");
	}
};