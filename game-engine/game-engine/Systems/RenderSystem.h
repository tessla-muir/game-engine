#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL.h>

class RenderSystem : public System {
	public:
		RenderSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<SpriteComponent>();
		}

		void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
			// Find all the relevant entities
			for (auto entity : GetEntities()) {
				const TransformComponent& transform = entity.GetComponent<TransformComponent>();
				const SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();

				// Create the source rect (sprite)
				SDL_Rect srcRect = {
					0,
					0,
					sprite.width,
					sprite.height
				};
				SDL_QueryTexture(assetStore->GetTexture(sprite.assetId), NULL, NULL, &srcRect.w, &srcRect.h);

				// Create the destination rect (what gets rendered)
				SDL_Rect destRect = {
					static_cast<int>(transform.position.x),
					static_cast<int>(transform.position.y),
					static_cast<int>(sprite.width * transform.scale.x),
					static_cast<int>(sprite.width * transform.scale.y)
				};
				// Default to sprite width & height if not given
				if (sprite.width == 0 && sprite.height == 0)
				{
					destRect.w = srcRect.w * transform.scale.x;
					destRect.h = srcRect.h * transform.scale.y;
				}

				SDL_RenderCopyEx(
					renderer,
					assetStore->GetTexture(sprite.assetId),
					&srcRect,
					&destRect,
					transform.rotation,
					NULL,
					SDL_FLIP_NONE
				);
			}
		}
};

#endif