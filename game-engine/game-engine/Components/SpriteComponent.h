#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	SDL_Rect srcRect;

	SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int srcX = 0, int srcY = 0) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->srcRect = { srcX, srcY, width, height };
	}
};

#endif