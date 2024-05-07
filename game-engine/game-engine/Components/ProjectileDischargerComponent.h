#ifndef PROJECTILEDISCHARGERCOMPONENT_H
#define PROJECTILEDISCHARGERCOMPONENT_H

#include <glm.hpp>
#include <SDL.h>

struct ProjectileDischargerComponent {
	glm::vec2 velocity;
	int dischargeFrequency;
	int lastDischargeTime;
	int projectileDuration;
	bool isFriendlyFire;

	ProjectileDischargerComponent(glm::vec2 velocity = glm::vec2(0), int frequency = 0, int duration = 10000, bool isFriendlyFire = false) {
		this->velocity = velocity;
		this->dischargeFrequency = frequency;
		this->projectileDuration = duration;
		this->isFriendlyFire = isFriendlyFire;
		this->lastDischargeTime = SDL_GetTicks();
	}
};

#endif