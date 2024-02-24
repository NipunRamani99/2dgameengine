#pragma once
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

#include <memory>
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	int millisecsPreviousFrame = 0;
	bool isRunning = true;
	int windowWidth = 800;
	int windowHeight = 600;
	bool fullscreen = false;
	std::unique_ptr<Registry> registry;
public:
	Game();
	~Game();
	void Initialize();
	void Destroy();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
};