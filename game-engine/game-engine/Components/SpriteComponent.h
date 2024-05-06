#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;

	SpriteComponent(std::string assetId = "", int width = 10, int height = 10) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
	}
};

#endif