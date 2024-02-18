#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../Logger/Logger.h"

Game::Game() {
	Logger::Log("Game constructor called.");
}

Game::~Game() {

	Logger::Log("Game destructor called.");
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
glm::vec2 playerPosition = { 10,20 };
glm::vec2 playerVelocity = { 1, 1 };
void Game::Setup() {
	playerPosition = { 10.0, 20.0 };
	playerVelocity = { 10.0, 50.0 };
	// TODO: initialize game objects..
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

	playerPosition += float(deltaTime) * playerVelocity;
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Draw a PNG..
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	size_t w = surface->w;
	size_t h = surface->h;
	SDL_FreeSurface(surface);

	
	SDL_Rect destRect = {
		static_cast<int>(playerPosition.x), 
		static_cast<int>(playerPosition.y), 
		w, 
		h};
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}
