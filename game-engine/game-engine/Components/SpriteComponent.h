#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

struct SpriteComponent {
	int width;
	int height;

	SpriteComponent(int width = 10, int height = 10) {
		this->width = width;
		this->height = height;
	}
};

#endif