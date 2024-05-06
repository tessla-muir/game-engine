#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"
#include <SDL.h>

class KeyboardControlSystem : public System {
	public: 
		KeyboardControlSystem() {
			
		}

		void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->ListenToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onKeyPressed);
		}

		void onKeyPressed(KeyPressedEvent& event) {
			Logger::Debug("KeyboardControlSystem.cs: Key [" + std::to_string(event.key) + "] pressed!");
		}

		void Update() {
			
		}
};

#endif