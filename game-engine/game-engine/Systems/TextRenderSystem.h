#ifndef TEXTRENDERSYSTEM_H
#define TEXTRENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TextComponent.h"
#include "../Components/TransformComponent.h"
#include "../AssetStore/AssetStore.h"

class RenderTextSystem : public System {
	public:
		RenderTextSystem() {
			RequireComponent<TextComponent>();
			RequireComponent<TransformComponent>();
		}

		void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
			for (auto entity : GetEntities()) {
				const TextComponent text = entity.GetComponent<TextComponent>();
				const TransformComponent transform = entity.GetComponent<TransformComponent>();

				// Create surface to display font
				SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont(text.assetId), text.text.c_str(), text.color);
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
				SDL_FreeSurface(surface);

				// Determine text's rectangle's width and height
				int width = 0;
				int height = 0;
				SDL_QueryTexture(texture, NULL, NULL, &width, &height);

				// Determine text's x based on alignment
				int posX = 0;

				if (text.alignment == Left) {
					posX = transform.position.x;
				}
				else if (text.alignment == Center) {
					posX = transform.position.x - width / 2;
				}
				else if (text.alignment == Right) {
					posX = transform.position.x - width;
				}

				// Rectangle that text goes into
				SDL_Rect destRect = {
					posX,
					transform.position.y,
					width,
					height
				};

				// Put text into renderer
				SDL_RenderCopy(renderer, texture, NULL, &destRect);
			}
		}
};

#endif