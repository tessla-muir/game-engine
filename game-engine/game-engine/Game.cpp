#include "Game.h"
#include "./Logger/Logger.h"
#include "./ECS/ECS.h"
#include <SDL.h>
#include <iostream>
#include "./Components/TransformComponent.h"
#include "./Components/RigidbodyComponent.h"
#include "./Components/BoxColliderComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/AnimationComponent.h"
#include "./Systems/MovementSystem.h"
#include "./Systems/KeyboardControlSystem.h"
#include "./Systems/CollisionSystem.h"
#include "./Systems/CollisionDebugSystem.h"
#include "./Systems/RenderSystem.h"
#include "./Systems/AnimationSystem.h"
#include "./Systems/DamageSystem.h"

Game::Game() {
	isRunning = false;
	isDebugging = false;
	compManager = std::make_unique<ComponentManager>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
}

Game::~Game() {
	
}

void Game::Setup() {
	// Add systems needed for the game
	compManager->AddSystem<MovementSystem>();
	compManager->AddSystem<KeyboardControlSystem>();
	compManager->AddSystem<RenderSystem>();
	compManager->AddSystem<AnimationSystem>();
	compManager->AddSystem<CollisionSystem>();
	compManager->AddSystem<CollisionDebugSystem>();
	compManager->AddSystem<DamageSystem>();

	// Add Assets
	assetStore->AddTexture(renderer, "invader1", "./Assets/Images/invader1.png");
	assetStore->AddTexture(renderer, "invader1a", "./Assets/Images/invader1a.png");
	assetStore->AddTexture(renderer, "invader1b", "./Assets/Images/invader1b.png");
	assetStore->AddTexture(renderer, "invader2", "./Assets/Images/invader2.png");

	Entity test = compManager->CreateEntity();
	test.AddComponent<TransformComponent>(glm::vec2(400.0, 400.0), glm::vec2(1.0, 1.0), 0.0);
	test.AddComponent<SpriteComponent>("invader2", 110, 100);
	test.AddComponent<AnimationComponent>(2, 1, true);
	test.AddComponent<RigidBodyComponent>(glm::vec2(-1, -1));
	test.AddComponent<BoxColliderComponent>(110, 100);

	Entity test2 = compManager->CreateEntity();
	test2.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	test2.AddComponent<SpriteComponent>("invader1", 100, 100);
	test2.AddComponent<AnimationComponent>(2, 1, true);
	test2.AddComponent<RigidBodyComponent>(glm::vec2(1, 1));
	test2.AddComponent<BoxColliderComponent>(100, 100);
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
			if (event.key.keysym.sym == SDLK_d) {
				isDebugging = !isDebugging;
			}
			eventBus->DispatchEvent<KeyPressedEvent>(event.key.keysym.sym);
			break;
		}
	}
}

void Game::Update() {
	int waitTime = FRAME_MILISECS - (SDL_GetTicks() - prevFrameMilisecs);
	if (waitTime > 0 && waitTime <= FRAME_MILISECS) {
		SDL_Delay(waitTime);
	}

	double deltaTime = (SDL_GetTicks() - prevFrameMilisecs) / 1000.0;

	// System Listeners
	// Suboptimal to listen each frame
	eventBus->Reset(); // Reset Listeners
	compManager->GetSystem<DamageSystem>().ListenToEvents(eventBus); // Establish listeners
	compManager->GetSystem<KeyboardControlSystem>().ListenToEvents(eventBus);

	// Update systems
	compManager->GetSystem<MovementSystem>().Update(deltaTime);
	compManager->GetSystem<AnimationSystem>().Update();
	compManager->GetSystem<CollisionSystem>().Update(eventBus);
	compManager->GetSystem<DamageSystem>().Update();
	//compManager->GetSystem<KeyboardControlSystem>().Update();

	// Update component manager
	compManager->Update();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Render objects
	compManager->GetSystem<RenderSystem>().Update(renderer, assetStore);
	if (isDebugging) compManager->GetSystem<CollisionDebugSystem>().Update(renderer);

	SDL_RenderPresent(renderer);
}

void Game::Destory() {
	// Destroy SDL items in reverse order
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	Logger::Log("Game.cpp: Game Ended");
}