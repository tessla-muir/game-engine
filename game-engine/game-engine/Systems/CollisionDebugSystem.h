#ifndef COLLISIONDEBUGSYSTEM_H
#define COLLISIONDEBUGSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include <SDL.h>

class CollisionDebugSystem : public System {
	public:
		CollisionDebugSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<BoxColliderComponent>();
		}

		void Update(SDL_Renderer* renderer) {
			// Get relevant entities
			for (auto entity : GetEntities()) {
				TransformComponent transform = entity.GetComponent<TransformComponent>();
				BoxColliderComponent collider = entity.GetComponent<BoxColliderComponent>();

				// Draw rect
				SDL_Rect cRect = {
					static_cast<int>(transform.position.x + collider.offset.x),
					static_cast<int>(transform.position.y + collider.offset.y),
					static_cast<int>(collider.width),
					static_cast<int>(collider.height)
				};
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(renderer, &cRect);
			}
	}
};

#endif