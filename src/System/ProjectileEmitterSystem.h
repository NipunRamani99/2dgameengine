#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
class ProjectileEmitterSystem : public System {
public:
	ProjectileEmitterSystem(Registry * registry) 
		:
		System(registry)
	{
		RequireComponent<TransformComponent>();
		RequireComponent<ProjectileEmitterComponent>();
		
	}
	
	void Update(std::unique_ptr<Registry> & registry, std::unique_ptr<AssetStore> & assetStore) {
		for (auto entity : GetEntities()) {
			ProjectileEmitterComponent & projectileEmitter = registry->GetComponent<ProjectileEmitterComponent>(entity);
			auto & transform = registry->GetComponent<TransformComponent>(entity);
			// TODO:
			// Check if its time to re-emity a new projectile
			if (SDL_GetTicks() - projectileEmitter.lastEmissionTime  > projectileEmitter.repeatFrequency) {
				// TODO: Add a new projectile entity to the registry 
				// registry->CreateEntity()...
				Entity projectile = registry->CreateEntity();
				registry->AddComponent<TransformComponent>(projectile, transform.position, transform.scale, transform.rotation);
				registry->AddComponent<SpriteComponent>(projectile, SDL_Rect{0,0,4,4}, assetStore->GetTexture("bullet"), 1);
				registry->AddComponent<AnimationComponent>(projectile, 1, 1000 / 12, false);
				registry->AddComponent<RigidBodyComponent>(projectile, projectileEmitter.projectileVelocity, glm::vec2{ 4.0,4.0 });
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
			}

		}
	}
};