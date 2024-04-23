#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

	public:
		Game();
		~Game();
		void Initalize();
		void Run();
		void ProcessInput();
		void Update();
		void Render();
		void Destory();
};

#endif