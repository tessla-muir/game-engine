#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <string>
#include <SDL.h>

enum Alignment {
	Left,
	Center,
	Right
};

struct TextComponent {
	std::string text;
	std::string assetId; // Font
	SDL_Color color;
	Alignment alignment;

	TextComponent(std::string text = "", std::string assetId = "", const SDL_Color color = { 0, 0, 0 }, Alignment alignment = Left) {
		this->text = text;
		this->assetId = assetId;
		this->color = color;
		this-> alignment = alignment;
	}
};

#endif