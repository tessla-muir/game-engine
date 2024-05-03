#include "Game.h"
#include "Logger.h"
#include <SDL.h>
#include <iostream>

Game::Game() {
	isRunning = false;
}

Game::~Game() {

}

void Game::Initalize() {
	// Intializes everything for SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Error("Game.cs: SDL Initialization Error");
		return;
	}

	window = SDL_CreateWindow(
		"2D Game Engine", // Window title
		SDL_WINDOWPOS_CENTERED, // Center the window on width & height
		SDL_WINDOWPOS_CENTERED,
		800, // Window resolution: width & height
		600,
		SDL_WINDOW_SHOWN // Show window
	);

	// Ensure window intialization
	if (!window) {
		Logger::Error("Game.cs: Window Initalization Error");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	// Ensure renderer intialization
	if (!renderer) {
		Logger::Error("Game.cs: Renderer Initalizatino Error");
		return;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Set game to running state
	isRunning = true;
	Logger::Log("Game.cpp: Game Started");
}

void Game::Run() {
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		// User can press X on window to quit
		case SDL_QUIT: {
			isRunning = false;
			break;
		}
		case SDL_KEYDOWN: {
			// User can press ESC key to quit
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
				break;
			}
		}
	}
}

void Game::Update() {

}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render objects

	SDL_RenderPresent(renderer);
}

void Game::Destory() {
	// Destroy SDL items in reverse order
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	Logger::Log("Game.cpp: Game Ended");
}