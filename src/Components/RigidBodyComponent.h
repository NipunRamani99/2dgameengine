#pragma once
#include <glm/glm.hpp>
struct RigidBodyComponent {
	glm::vec2 velocity;
	glm::vec2 dim;

	RigidBodyComponent() = default;

	RigidBodyComponent(glm::vec2 velocity, glm::vec2 dim = {32.0f, 32.0f})
		:
		velocity(velocity),
		dim(dim)
	{
	
	}
};