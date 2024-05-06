#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "./EventBus/Event.h"
#include <SDL.h>

class KeyPressedEvent : public Event {
	public:
		SDL_Keycode key;
		bool isDown = false;

		KeyPressedEvent(SDL_Keycode key, bool isDown) {
			this->key = key;
			this->isDown = isDown;
		}
};

#endif