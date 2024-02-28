#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"
class MovementSystem : public System {
public:
	MovementSystem(Registry * registry) 
		:
		System(registry)
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		Logger::Log("Initialized MovementSystem");
	}

	void Update(float deltaTime) {
		//Loop all entities that the system is interested in
		for (auto entity : GetEntities()) {
			auto& transformComponent = registry->GetComponent<TransformComponent>(entity);
			const auto & rigidBodyComponent = registry->GetComponent<RigidBodyComponent>(entity);
			transformComponent.position += deltaTime * rigidBodyComponent.velocity;
		}
	}

	~MovementSystem() {
		Logger::Log("MovementSystem destructed.");
	}
};