#pragma once
#include <glm/glm.hpp>
struct RigidBodyComponent {
	glm::vec2 velocity;

	RigidBodyComponent() = default;

	RigidBodyComponent(glm::vec2 velocity) 
		:
		velocity(velocity)
	{
	
	}
};