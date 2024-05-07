#include "Game.h"
#include "./Logger/Logger.h"
#include "./ECS/ECS.h"
#include <SDL.h>
#include "./Components/TransformComponent.h"
#include "./Components/RigidbodyComponent.h"
#include "./Components/BoxColliderComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/AnimationComponent.h"
#include "./Components/KeyboardControlledComponent.h"
#include "./Components/ProjectileDischargerComponent.h";
#include "./Components/PlayerComponent.h"
#include "./Systems/MovementSystem.h"
#include "./Systems/KeyboardControlSystem.h"
#include "./Systems/CollisionSystem.h"
#include "./Systems/CollisionDebugSystem.h"
#include "./Systems/RenderSystem.h"
#include "./Systems/AnimationSystem.h"
#include "./Systems/DamageSystem.h"
#include "./Systems/LifetimeSystem.h"
#include "./Systems/ProjectileDischargeSystem.h"

const int WIN_WIDTH = 980;
const int WIN_HEIGHT = 980;

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
	compManager->AddSystem<ProjectileDischargeSystem>();
	compManager->AddSystem<LifetimeSystem>();

	// Add Assets
	assetStore->AddTexture(renderer, "ship", "./Assets/Images/ship.png");
	assetStore->AddTexture(renderer, "invader1", "./Assets/Images/invader1.png");
	assetStore->AddTexture(renderer, "invader2", "./Assets/Images/invader2.png");
	assetStore->AddTexture(renderer, "invader3", "./Assets/Images/invader3.png");
	assetStore->AddTexture(renderer, "projectile1", "./Assets/Images/projectile1.png");

	// Player
	Entity player = compManager->CreateEntity();
	player.AddComponent<TransformComponent>(glm::vec2(WIN_WIDTH/2 - 55, WIN_HEIGHT-200), glm::vec2(2.0, 2.0), 0.0);
	player.AddComponent<SpriteComponent>("ship", 30, 30);
	player.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
	player.AddComponent<BoxColliderComponent>(30 * 2.0, 30 * 2.0);
	player.AddComponent<KeyboardControlledComponent>(200, true, false);
	player.AddComponent<ProjectileDischargerComponent>(glm::vec2(0.0, -200.0), 0, 4000, 500);
	player.Tag("Player");

	// Aliens
	Entity test1 = compManager->CreateEntity();
	test1.AddComponent<TransformComponent>(glm::vec2(200.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
	test1.AddComponent<SpriteComponent>("invader2",30, 30);
	test1.AddComponent<AnimationComponent>(2, 1, true);
	test1.AddComponent<BoxColliderComponent>(30 * 2.0, 30 * 2.0);
	test1.AddComponent<ProjectileDischargerComponent>(glm::vec2(0.0, 100.0), 8000, 8000);
	test1.Group("Enemy");

	Entity test2 = compManager->CreateEntity();
	test2.AddComponent<TransformComponent>(glm::vec2(700.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
	test2.AddComponent<SpriteComponent>("invader1", 30, 30);
	test2.AddComponent<AnimationComponent>(2, 1, true);
	test2.AddComponent<BoxColliderComponent>(30 * 2.0, 30 * 2.0);
	test2.AddComponent<ProjectileDischargerComponent>(glm::vec2(0.0, 100.0), 6000, 8000);
	test2.Group("Enemy");

	Entity test3 = compManager->CreateEntity();
	test3.AddComponent<TransformComponent>(glm::vec2(450.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
	test3.AddComponent<SpriteComponent>("invader3", 30, 30);
	test3.AddComponent<AnimationComponent>(2, 1, true);
	test3.AddComponent<BoxColliderComponent>(30 * 2.0, 30 * 2.0);
	test3.Group("Enemy");
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
		WIN_WIDTH, // Window resolution: width & height
		WIN_HEIGHT,
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
			if (event.key.keysym.sym == SDLK_p) {
				isDebugging = !isDebugging;
			}
			eventBus->DispatchEvent<KeyPressedEvent>(event.key.keysym.sym, true);
			break;
		}
		case SDL_KEYUP: {
			eventBus->DispatchEvent<KeyPressedEvent>(event.key.keysym.sym, false);
		}
	}
}

void Game::Update() {
	// Calculate the time elapsed since the last frame
	Uint32 currentTicks = SDL_GetTicks();
	double deltaTime = (currentTicks - prevFrameMilisecs) / 1000.0;

	// Update prevFrameMilisecs for the next frame
	prevFrameMilisecs = currentTicks;

	// Cap deltaTime to avoid large jumps in case of a long frame
	deltaTime = (deltaTime < 30) ? deltaTime : 30;

	// System Listeners
	// Suboptimal to listen each frame
	eventBus->Reset(); // Reset Listeners
	compManager->GetSystem<DamageSystem>().ListenToEvents(eventBus); // Establish listeners
	compManager->GetSystem<KeyboardControlSystem>().ListenToEvents(eventBus);
	compManager->GetSystem<ProjectileDischargeSystem>().ListenToEvents(eventBus);

	// Update systems
	compManager->GetSystem<MovementSystem>().Update(deltaTime);
	compManager->GetSystem<AnimationSystem>().Update();
	compManager->GetSystem<CollisionSystem>().Update(eventBus);
	compManager->GetSystem<LifetimeSystem>().Update();
	compManager->GetSystem<ProjectileDischargeSystem>().Update();

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