#pragma once
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/EventBus.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem : public System{
private:
	
public:

	CollisionSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	CollisionSystem(Registry * registry) 
		:
		System(registry)
	{
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update(float dt, std::unique_ptr<EventBus> & eventBus) {
		auto entities = GetEntities();
		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity a = *i;
			auto aTransform = registry->GetComponent<TransformComponent>(a);
			auto aCollider = registry->GetComponent<BoxColliderComponent>(a);

			for (auto j = i + 1; j != entities.end(); j++) {
				Entity b = *j;
				if (a == b) continue;
				auto bTransform = registry->GetComponent<TransformComponent>(b);
				auto bCollider = registry->GetComponent<BoxColliderComponent>(b);
				

				if (CheckAABBCollision(aTransform.position.x, aTransform.position.y, aCollider.width * aTransform.scale.x, aCollider.height * aTransform.scale.y,
									   bTransform.position.x, bTransform.position.y, bCollider.width * bTransform.scale.x, bCollider.height * bTransform.scale.y)) {

					Logger::Log("Collision Detected Between Entity ID: " + std::to_string(a.GetId()) + " and Entity ID: " + std::to_string(b.GetId()));

					CollisionEvent event(a, b);
				   eventBus->publishEvent<CollisionEvent>(&event);
				}
			}
		}
	}


	bool CheckAABBCollision(double aX, double aY, double aW, double aH, 
		double bX, double bY, double bW, double bH) {
		
		return (
				aX < bX + bW &&
				aX + aW > bX &&
				aY < bY + bH &&
				aY + aH > bY
			);

	}

};