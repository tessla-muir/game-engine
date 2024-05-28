#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <string>
#include <SDL.h>

struct TextComponent {
	std::string text;
	std::string assetId; // Font
	SDL_Color color;

	TextComponent(std::string text = "", std::string assetId = "", const SDL_Color color = { 0, 0, 0 }) {
		this->text = text;
		this->assetId = assetId;
		this->color = color;
	}
};

#endif