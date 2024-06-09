#include "Game.h"
#include "./Logger/Logger.h"
#include "./Debugger/Debugger.h"
#include "./UnitTests/UnitTests.h"
#include "./ECS/ECS.h"
#include <SDL.h>
#include "./Components/TransformComponent.h"
#include "./Components/RigidbodyComponent.h"
#include "./Components/BoxColliderComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/AnimationComponent.h"
#include "./Components/KeyboardControlledComponent.h"
#include "./Components/ProjectileDischargerComponent.h";
#include "./Components/TextComponent.h"
#include "./Systems/MovementSystem.h"
#include "./Systems/KeyboardControlSystem.h"
#include "./Systems/CollisionSystem.h"
#include "./Systems/CollisionDebugSystem.h"
#include "./Systems/RenderSystem.h"
#include "./Systems/AnimationSystem.h"
#include "./Systems/DamageSystem.h"
#include "./Systems/LifetimeSystem.h"
#include "./Systems/ProjectileDischargeSystem.h"
#include "./Systems/ScoreSystem.h"
#include "./Systems/TextRenderSystem.h"

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
	compManager->AddSystem<DamageSystem>(eventBus);
	compManager->AddSystem<ProjectileDischargeSystem>();
	compManager->AddSystem<LifetimeSystem>();
	compManager->AddSystem<ScoreSystem>();
	compManager->AddSystem<TextRenderSystem>();

	// Add Assets -- Textures
	assetStore->AddTexture(renderer, "ship", "./Assets/Images/ship.png");
	assetStore->AddTexture(renderer, "invader1", "./Assets/Images/invader1.png");
	assetStore->AddTexture(renderer, "invader2", "./Assets/Images/invader2.png");
	assetStore->AddTexture(renderer, "invader3", "./Assets/Images/invader3.png");
	assetStore->AddTexture(renderer, "projectile1", "./Assets/Images/projectile1.png");
	assetStore->AddTexture(renderer, "projectile2", "./Assets/Images/projectile2.png");

	// Add Assets -- Fonts
	assetStore->AddFont("ATROX-font", "./Assets/Fonts/ATROX.ttf", 50);

	LoadLevel();
}

void Game::LoadLevel() {
	// Player
	Entity player = compManager->CreateEntity();
	player.AddComponent<TransformComponent>(glm::vec2(WIN_WIDTH / 2 - 55, WIN_HEIGHT - 200), glm::vec2(2.0, 2.0), 0.0);
	player.AddComponent<SpriteComponent>("ship", 30, 30);
	player.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
	player.AddComponent<BoxColliderComponent>(30 * 2.0, 30 * 2.0);
	player.AddComponent<KeyboardControlledComponent>(200, true, false);
	player.AddComponent<ProjectileDischargerComponent>(glm::vec2(0.0, -200.0), 0, 4000, 500);
	player.AddComponent<ScoreComponent>(0);
	compManager->GetSystem<ScoreSystem>().SetPlayerEntity(player);
	player.Tag("Player");

	// Alien grid
	int rows = 5;
	int columns = 11;
	int gap = 15;
	glm::vec2 initialPosition(100, 100);

	if (UnitTests::unitTest > 0 && UnitTests::unitTest <= 3) {
		rows += UnitTests::unitTest;
	}

	// Add aliens
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < columns; col++) {
			// Find position
			glm::vec2 position = initialPosition + glm::vec2((60 + gap) * col, 60 * row);

			// Determine sprite & score to use
			std::string spriteName;
			int score;
			if (row == 0) {
				spriteName = "invader3";
				score = 30;
			}
			else if (row <= 2) {
				spriteName = "invader2";
				score = 20;
			}
			else {
				spriteName = "invader1";
				score = 10;
			}

			// Create entity to add
			Entity alien = compManager->CreateEntity();
			alien.AddComponent<TransformComponent>(position, glm::vec2(2.0, 2.0), 0.0);
			alien.AddComponent<SpriteComponent>(spriteName, 30, 30);
			alien.AddComponent<AnimationComponent>(2, 1, true);
			alien.AddComponent<BoxColliderComponent>(30 * 2.0, 30 * 2.0);
			alien.AddComponent<ProjectileDischargerComponent>(glm::vec2(0.0, 200.0), 0, 4000, 0, 1000, 40000);
			alien.AddComponent<ScoreComponent>(score);
			alien.Group("Enemy");
		}
	}

	Entity title = compManager->CreateEntity();
	SDL_Color white = { 255, 255, 255 };
	title.AddComponent<TextComponent>("Space Invaders", "ATROX-font", white, Center);
	title.AddComponent<TransformComponent>(glm::vec2(WIN_WIDTH / 2, 50));
}

void Game::Initalize() {
	// Intializes everything for SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Error("Game.cs: SDL Initialization Error");
		return;
	}

	// Intalize SDL fonts
	if (TTF_Init() != 0) {
		Logger::Error("Game.cs: SDL Font Intalization Error");
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
	compManager->GetSystem<ScoreSystem>().ListenToEvents(eventBus);

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
	if (isDebugging || (Debugger::debugLevel == 5 || Debugger::debugLevel == 9)) compManager->GetSystem<CollisionDebugSystem>().Update(renderer);

	// Render UI
	compManager->GetSystem<TextRenderSystem>().Update(renderer, assetStore);

	SDL_RenderPresent(renderer);
}

void Game::Destory() {
	// Destroy SDL items in reverse order
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	Logger::Log("Game.cpp: Game Ended");
}