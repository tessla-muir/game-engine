#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>

class RenderSystem : public System {
	public:
		RenderSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<SpriteComponent>();
		}

		void Update(SDL_Renderer* renderer) {
			// Find all the relevant entities
			for (auto entity : GetEntities()) {
				const TransformComponent& transform = entity.GetComponent<TransformComponent>();
				const SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

				// Draw 'sprite'
				SDL_Rect rect = {
					static_cast<int>(transform.position.x),
					static_cast<int>(transform.position.y),
					sprite.width,
					sprite.height
				};

				// Set color and render it
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
};

#endif