#ifndef PROJECTILEDISCHARGERCOMPONENT_H
#define PROJECTILEDISCHARGERCOMPONENT_H

#include <glm.hpp>
#include <SDL.h>

// Entity that actually discharges the projectiles
struct ProjectileDischargerComponent {
	glm::vec2 velocity;
	int lastDischargeTime;
	int projectileDuration;

	// Firing rates
	int fireRate; // Keyboard Controlled (Player)
	int dischargeFrequency; // Any Entity

	// Random fire interval
	int min;
	int max;
	int randomTime;

	ProjectileDischargerComponent(glm::vec2 velocity = glm::vec2(0), int frequency = 0, int duration = 10000, int fireRate = 0, int min = 0, int max = 0) {
		this->velocity = velocity;
		this->projectileDuration = duration;

		// Fire rates
		this->dischargeFrequency = frequency;
		this->fireRate = fireRate;

		// Random
		// Determine random time
		this->min = min;
		this->max = max;
		this->randomTime = min + rand() % (max - min + 1);

		this->lastDischargeTime = SDL_GetTicks();
	}
};

#endif