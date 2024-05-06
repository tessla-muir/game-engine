#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"
#include <SDL.h>

class KeyboardControlSystem : public System {
	public: 
		KeyboardControlSystem() {
			RequireComponent<KeyboardControlledComponent>();
			RequireComponent<RigidBodyComponent>();
			RequireComponent<SpriteComponent>();
		}

		void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->ListenToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onKeyPressed);
		}

		void onKeyPressed(KeyPressedEvent& event) {
			for (auto entity : GetEntities()) {
				const KeyboardControlledComponent keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
				RigidBodyComponent& rigidbody = entity.GetComponent<RigidBodyComponent>();

				if (event.key == SDLK_UP || event.key == SDLK_w) {
					rigidbody.velocity = keyboardcontrol.upVelocity;
				}
				else if (event.key == SDLK_DOWN || event.key == SDLK_s) {
					rigidbody.velocity = keyboardcontrol.downVelocity;
				}
				if (event.key == SDLK_LEFT || event.key == SDLK_a) {
					rigidbody.velocity = keyboardcontrol.leftVelocity;
				}
				else if (event.key == SDLK_RIGHT || event.key == SDLK_d) {
					rigidbody.velocity = keyboardcontrol.rightVelocity;
				}
			}
		}

		void Update() {
			
		}
};

#endif