#pragma once
#include <glm/glm.hpp>
struct TransformComponent {
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;

	TransformComponent() 
		:
		position(0,0),
		scale(0,0),
		rotation(0)
	{
	}

	TransformComponent(glm::vec2 position, glm::vec2 scale, double rotation)
		:
		position(position),
		scale(scale),
		rotation(rotation) 
	{
		
	}
};