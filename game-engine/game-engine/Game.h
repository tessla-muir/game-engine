#ifndef GAME_H
#define GAME_H

#include "./ECS/ECS.h"
#include "./AssetStore/AssetStore.h"
#include <SDL.h>

const int FPS = 60;
const int FRAME_MILISECS = (1000 / FPS);

class Game {
private:
	bool isRunning;
	int prevFrameMilisecs = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unique_ptr<ComponentManager> compManager;
	std::unique_ptr<AssetStore> assetStore;

	public:
		Game();
		~Game();
		void Initalize();
		void Setup();
		void Run();
		void ProcessInput();
		void Update();
		void Render();
		void Destory();
};

#endif