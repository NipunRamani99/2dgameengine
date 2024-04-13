#pragma once
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Events/EventBus.h"
#include <memory>
#include "../Utils/Constants.h"
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	int millisecsPreviousFrame = 0;
	bool isRunning = true;
	bool fullscreen = false;
	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;
public:
	static int windowWidth;
	static int windowHeight;
	static int mapWidth;
	static int mapHeight;


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