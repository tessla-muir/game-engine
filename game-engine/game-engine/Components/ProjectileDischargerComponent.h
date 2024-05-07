#ifndef PROJECTILEDISCHARGERCOMPONENT_H
#define PROJECTILEDISCHARGERCOMPONENT_H

#include <glm.hpp>
#include <SDL.h>

// Entity that actually discharges the projectiles
struct ProjectileDischargerComponent {
	glm::vec2 velocity;
	int dischargeFrequency;
	int lastDischargeTime;
	int projectileDuration;
	int fireRate;

	ProjectileDischargerComponent(glm::vec2 velocity = glm::vec2(0), int frequency = 0, int duration = 10000, int fireRate = 0) {
		this->velocity = velocity;
		this->dischargeFrequency = frequency;
		this->projectileDuration = duration;
		this->fireRate = fireRate;
		this->lastDischargeTime = SDL_GetTicks();
	}
};

#endif