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
#include "../System/RenderSystem.h"
Game::Game() {
	Logger::Log("Game constructed.");
	registry = std::make_unique<Registry>();
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
		windowWidth = 800;//displayMode.w;
		windowHeight = 600;//displayMode.h;
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
	assetStore->AddTexture("tank","./assets/images/dvdlogo.png");


	Entity tank = registry->CreateEntity();
	registry->AddComponent<TransformComponent>(tank, glm::vec2{ 10.0,10.0 }, glm::vec2{ 0.10, 0.10 }, 0.0);
	registry->AddComponent<RigidBodyComponent>(tank, glm::vec2{ 50.0, 50.0 });
	registry->AddComponent<SpriteComponent>(tank, SDL_Rect{0, 0, 840, 477}, assetStore->GetTexture("tank"));
	registry->AddSystem<MovementSystem>(registry.get());
	registry->AddSystem<RenderSystem>(registry.get(), renderer);
	
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
			}
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
	registry->Update((float)deltaTime);

}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render Game Objects...
	registry->GetSystem<RenderSystem>().Update(1/60.0f);
	SDL_RenderPresent(renderer);
}
