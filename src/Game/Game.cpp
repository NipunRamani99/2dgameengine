#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../System/MovementSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TileComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../System/RenderSystem.h"
#include "../System/TileMapRenderSystem.h"
#include "../System/AnimationSystem.h"
#include "../System/CollisionSystem.h"
#include "../System/KeyboardMovementSystem.h"
#include <ctime>
#include <stdlib.h>
Game::Game() {
	Logger::Log("Game constructed.");
	registry = std::make_unique<Registry>();
	eventBus = std::make_unique<EventBus>();
}

Game::~Game() {
	Logger::Log("Game destructed.");
}


void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL.");
		return;
	}

	if (windowWidth < 0 || windowHeight < 0) {
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		windowWidth = 1000;//displayMode.w;
		windowHeight =1000;//displayMode.h;
		fullscreen = true;
	}
	window = SDL_CreateWindow("SDL Window", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		windowWidth, 
		windowHeight, 
		NULL);
	if (!window) {
		Logger::Err("Error creating SDL window.");
		return;
	}
	renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		Logger::Err("Error creating SDL renderer.");
		return;
	}
	if (fullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	

	
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run() {
	Setup();
	while(isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Setup() {

	// TODO: initialize game objects..

	//Initialize the ECS system
	/*
	* Entity tank = registry.CreateEntity();
	* tank.AddComponent<TransformComponent>();
	* tank.AddComponent<BoxColliderComponent>();
	* tank.AddComponent<SpriteComponent>();
	* TransformComponent component = registry.createComponent<TransformComponent>({50.0,50.0},{1.0,1.0},0.0f);
	* 
	* registry.registerComponent(tank, component);
	*/
	assetStore = std::make_unique<AssetStore>(renderer);
	assetStore->AddTexture("chopper","./assets/images/chopper-spritesheet.png");
	assetStore->AddTexture("jungle_tilemap", "./assets/tilemaps/jungle.png");
	assetStore->AddTexture("tank", "./assets/images/tank-tiger-left.png");
	assetStore->AddTexture("truck", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture("dvd", "./assets/images/dvdlogo.png");


	Entity chopper = registry->CreateEntity();
	registry->AddComponent<TransformComponent>(chopper, glm::vec2{ 100.0,100.0 }, glm::vec2{ 1.0, 1.0}, 0.0);
	registry->AddComponent<RigidBodyComponent>(chopper, glm::vec2{ 0.0, 0.0 });
	registry->AddComponent<SpriteComponent>(chopper, SDL_Rect{0, 0, 32, 32}, assetStore->GetTexture("chopper"), 1);
	registry->AddComponent<KeyboardMovementComponent>(chopper, glm::vec2{0.0, -20.0}, glm::vec2{20.0, 0.0}, glm::vec2{0.0, 20.0}, glm::vec2{-20.0, 0.0});
	registry->AddComponent<AnimationComponent>(chopper, 2, 1000 / 12, true);

	Entity tank = registry->CreateEntity();
	registry->AddComponent<TransformComponent>(tank, glm::vec2{ 350.0, 100.0 }, glm::vec2{ 1.0, 1.0 }, 0.0);
	registry->AddComponent<RigidBodyComponent>(tank, glm::vec2{ -50.0, 0.0 });
	registry->AddComponent<SpriteComponent>(tank, SDL_Rect{ 0, 0, 32, 32 }, assetStore->GetTexture("tank"), 1);
	registry->AddComponent<BoxColliderComponent>(tank, 32, 32, glm::vec2{ 0.0,0.0 });
	registry->AddComponent<AnimationComponent>(tank, 1, 1000 / 12, false);

	Entity truck = registry->CreateEntity();
	registry->AddComponent<TransformComponent>(truck, glm::vec2{ 150.0, 100.0 }, glm::vec2{ 1.0, 1.0 }, 0.0);
	registry->AddComponent<RigidBodyComponent>(truck, glm::vec2{ 50.0, 0.0 });
	registry->AddComponent<SpriteComponent>(truck, SDL_Rect{ 0, 0, 32, 32 }, assetStore->GetTexture("truck"), 1);
	registry->AddComponent<BoxColliderComponent>(truck, 32, 32, glm::vec2{ 0.0,0.0 });
	registry->AddComponent<AnimationComponent>(truck, 1, 1000 / 12, false);

	// Seed the random number generator (call this once at the beginning of your program)
	std::srand(static_cast<unsigned>(std::time(0)));

	// Random position
	float randomPosX = static_cast<float>(std::rand() % 200) + 100; // Example: random position between 0 and 800
	float randomPosY = static_cast<float>(std::rand() % 200) + 100; // Example: random position between 0 and 400

	// Random velocity
	float randomVelocityX = static_cast<float>(std::rand() % 100 + 50); // Example: random velocity between 50 and 150
	float randomVelocityY = static_cast<float>(std::rand() % 100 + 50);

	Entity dvd = registry->CreateEntity();
	registry->AddComponent<TransformComponent>(dvd, glm::vec2{ randomPosX, randomPosY }, glm::vec2{ 0.3, 0.3 }, 0.0);
	registry->AddComponent<RigidBodyComponent>(dvd, glm::vec2{ randomVelocityX, randomVelocityY }, glm::vec2{ 840, 480 });
	registry->AddComponent<SpriteComponent>(dvd, SDL_Rect{ 0, 0, 840, 477 }, assetStore->GetTexture("dvd"), 1);
	registry->AddComponent<BoxColliderComponent>(dvd, 840, 480, glm::vec2{ 0.0, 0.0 });
	registry->AddComponent<AnimationComponent>(dvd, 1, 1000 / 12, false);
	float tileScale = 1.50;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 10; x++) {
			Entity tile = registry->CreateEntity();
			registry->AddComponent<SpriteComponent>(tile, SDL_Rect{ x * 32, y * 32, 32, 32 }, assetStore->GetTexture("jungle_tilemap"), 0);
			registry->AddComponent<TileComponent>(tile, "jungle_tilemap", x + y * 10, glm::vec2{ tileScale, tileScale });
		}
	}
	
	registry->AddSystem<MovementSystem>(registry.get(), eventBus.get());
	registry->AddSystem<RenderSystem>(registry.get(), renderer);
	registry->AddSystem<AnimationSystem>(registry.get());
	registry->AddSystem<TileMapRenderSystem>(registry.get(), renderer);
	registry->GetSystem<TileMapRenderSystem>().LoadTileMap();
	registry->AddSystem<CollisionSystem>(registry.get());
	registry->AddSystem<KeyboardMovementSystem>(registry.get(), eventBus);
	

}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
				break;
			}
		case SDL_KEYUP:
			SDL_KeyboardEvent& keyEvent = sdlEvent.key;
			KeyboardEvent keyboardEvent{ 
				keyEvent.keysym.sym,
				keyEvent.keysym.scancode,
				keyEvent.keysym.mod,
				keyEvent.state == SDL_PRESSED,
				keyEvent.repeat != 0

			};
			eventBus->publishEvent<KeyboardEvent>(&keyboardEvent);
			break;
		}
	}

}


void Game::Update() {
	// TODO: if we are too fast, waste some time untill we reach the MILLISECS_PER_FRAME
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}
	
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// Store the current frame time
	millisecsPreviousFrame = SDL_GetTicks();

	// TODO:
	// MovementSystem.Update();
	// CollisionSystem.Update();
	// DamageSystem.Update();
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update(deltaTime);
	registry->GetSystem<CollisionSystem>().Update(deltaTime, eventBus);

	registry->Update((float)deltaTime);

}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render Game Objects...
	registry->GetSystem<TileMapRenderSystem>().Update(1/60.0f);

	registry->GetSystem<RenderSystem>().Update(1/60.0f);

	SDL_RenderPresent(renderer);
}
