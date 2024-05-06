#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "./EventBus/Event.h"
#include <SDL.h>

class KeyPressedEvent : public Event {
	public:
		SDL_Keycode key;

		KeyPressedEvent(SDL_Keycode key) {
			this->key = key;
		}
};

#endif