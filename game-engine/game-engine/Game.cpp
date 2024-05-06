#include "Game.h"
#include "./Logger/Logger.h"
#include "./ECS/ECS.h"
#include <SDL.h>
#include <iostream>
#include "./Components/TransformComponent.h"
#include "./Components/RigidbodyComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Systems/MovementSystem.h"
#include "./Systems/RenderSystem.h"

Game::Game() {
	isRunning = false;
	compManager = std::make_unique<ComponentManager>();
	assetStore = std::make_unique<AssetStore>();
}

Game::~Game() {
	
}

void Game::Setup() {
	// Add systems needed for the game
	compManager->AddSystem<MovementSystem>();
	compManager->AddSystem<RenderSystem>();

	// Add Assets
	assetStore->AddTexture(renderer, "invader1a", "./Assets/Images/invader1a.png");
	assetStore->AddTexture(renderer, "invader1b", "./Assets/Images/invader1b.png");

	Entity test = compManager->CreateEntity();
	test.AddComponent<TransformComponent>(glm::vec2(2.0, 2.0), glm::vec2(1.0, 1.0), 0.0);
	test.AddComponent<RigidBodyComponent>(glm::vec2(1.0, 1.0));
	test.AddComponent<SpriteComponent>("invader1a");

	Entity test2 = compManager->CreateEntity();
	test2.AddComponent<TransformComponent>(glm::vec2(20.0, 20.0), glm::vec2(1.0, 1.0), 0.0);
	test2.AddComponent<RigidBodyComponent>(glm::vec2(1.5, 0.2));
	test2.AddComponent<SpriteComponent>("invader1b");
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
	int waitTime = FRAME_MILISECS - (SDL_GetTicks() - prevFrameMilisecs);
	if (waitTime > 0 && waitTime <= FRAME_MILISECS) {
		SDL_Delay(waitTime);
	}

	double deltaTime = (SDL_GetTicks() - prevFrameMilisecs) / 1000.0;

	// Update systems
	compManager->GetSystem<MovementSystem>().Update(deltaTime);

	compManager->Update();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render objects
	compManager->GetSystem<RenderSystem>().Update(renderer, assetStore);

	SDL_RenderPresent(renderer);
}

void Game::Destory() {
	// Destroy SDL items in reverse order
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	Logger::Log("Game.cpp: Game Ended");
}