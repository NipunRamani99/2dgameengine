#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
struct ProjectileEmitterComponent {
	glm::vec2 projectileVelocity;
	int repeatFrequency;
	int projectileDuration;
	int hitPercentDamage;
	bool isFriendly;
	int lastEmissionTime;

	ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(10,10), int repeatFrequency = 1000, int projectileDuration = 10000, int hitPercentDamage = 10, bool isFriendly = false) {
		this->projectileVelocity = projectileVelocity;
		this->repeatFrequency = repeatFrequency;
		this->projectileVelocity = projectileVelocity;
		this->projectileDuration = projectileDuration;
		this->hitPercentDamage = hitPercentDamage;
		this->isFriendly = isFriendly;
		this->lastEmissionTime = SDL_GetTicks();
	}
};
