#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include <SDL.h>
#include "../Logger/Logger.h"

class AnimationSystem : public System {
	public:
		AnimationSystem() {
			RequireComponent<SpriteComponent>();
			RequireComponent<AnimationComponent>();
		}

		void Update() {
			// Find all the relevant entities
			for (auto entity : GetEntities()) {
				SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
				AnimationComponent& animation = entity.GetComponent<AnimationComponent>();

				// Gets a frame between 0 and numFrames-1
				animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.animSpeed / 1000) % animation.numFrames;

				// Change where we're viewing the rectangle
				// Logger::Log(std::to_string(sprite.width));
				sprite.srcRect.x = animation.currentFrame * sprite.width;
			}
		}
};

#endif