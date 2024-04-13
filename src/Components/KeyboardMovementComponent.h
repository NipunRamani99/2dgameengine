#pragma once
#include <glm/glm.hpp>

struct KeyboardMovementComponent {

	glm::vec2 velocities[4] = { {0.0,0.0} };

	KeyboardMovementComponent() {
		velocities[0] = {0.0,0.0};
		velocities[1] = {0.0,0.0};
		velocities[2] = {0.0,0.0};
		velocities[3] = {0.0,0.0};
	}

	KeyboardMovementComponent(glm::vec2 upVelocity, glm::vec2 rightVelocity, glm::vec2 downVelocity, glm::vec2 leftVelocity)
	{
		velocities[0] = upVelocity;
		velocities[1] = rightVelocity;
		velocities[2] = downVelocity;
		velocities[3] = leftVelocity;
	}
};