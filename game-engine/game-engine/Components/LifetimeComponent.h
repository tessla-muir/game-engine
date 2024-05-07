#ifndef LIFETIMECOMPONENT_H
#define LIFETIMECOMPONENT_H

#include <SDL.h>

struct LifetimeComponent {
	int duration;
	int startTime;

	LifetimeComponent(int duration = 0) {
		this->duration = duration;
		this->startTime = SDL_GetTicks();
	}
};

#endif