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

	ProjectileDischargerComponent(glm::vec2 velocity = glm::vec2(0), int frequency = 0, int duration = 10000) {
		this->velocity = velocity;
		this->dischargeFrequency = frequency;
		this->projectileDuration = duration;
		this->lastDischargeTime = SDL_GetTicks();
	}
};

#endif