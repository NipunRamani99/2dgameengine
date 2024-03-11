#pragma once
#include <glm/glm.hpp>

struct KeyboardMovementComponent {
	glm::vec2 upVelocity;
	glm::vec2 rightVelocity;
	glm::vec2 downVelocity;
	glm::vec2 leftVelocity;

	KeyboardMovementComponent() {}

	KeyboardMovementComponent(glm::vec2 upVelocity, glm::vec2 rightVelocity, glm::vec2 downVelocity, glm::vec2 leftVelocity)
		:
		upVelocity(upVelocity),
		rightVelocity(rightVelocity),
		downVelocity(downVelocity),
		leftVelocity(leftVelocity)
	{
	}
};