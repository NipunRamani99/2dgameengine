#pragma once
struct SpriteComponent {
	int width = 0;
	int height = 0;

	SpriteComponent() = default;
	SpriteComponent(int width, int height) : width(width), height(height) {}
};