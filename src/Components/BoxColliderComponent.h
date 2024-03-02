#pragma once
#include <glm/glm.hpp>
struct BoxColliderComponent {
	int width = 0;
	int height = 0;
	glm::vec2 offset = { 0.0, 0.0 };

	BoxColliderComponent() {}

	BoxColliderComponent(int width, int height, glm::vec2 offset)
		:
		width(width),
		height(height),
		offset(offset)
	{
	}
};