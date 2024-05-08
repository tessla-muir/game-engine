#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Debugger/Debugger.h"
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

                // Key Down Logic
                if (event.isDown) {
                    if (keyboardcontrol.allowVerticalMovement) {
                        if (event.key == SDLK_UP || event.key == SDLK_w) {
                            rigidbody.velocity.y = keyboardcontrol.upVelocity.y;
                            if (Debugger::debugLevel == 8 || Debugger::debugLevel == 9) Debugger::Log("KeyboardControlSystem: Player Up");
                        }
                        else if (event.key == SDLK_DOWN || event.key == SDLK_s) {
                            rigidbody.velocity.y = keyboardcontrol.downVelocity.y;
                            if (Debugger::debugLevel == 8 || Debugger::debugLevel == 9) Debugger::Log("KeyboardControlSystem: Player Down");
                        }
                    }

                    if (keyboardcontrol.allowHorizontalMovement) {
                        if (event.key == SDLK_LEFT || event.key == SDLK_a) {
                            rigidbody.velocity.x = keyboardcontrol.leftVelocity.x;
                            if (Debugger::debugLevel == 8 || Debugger::debugLevel == 9) Debugger::Log("KeyboardControlSystem: Player Left");
                        }
                        else if (event.key == SDLK_RIGHT || event.key == SDLK_d) {
                            rigidbody.velocity.x = keyboardcontrol.rightVelocity.x;
                            if (Debugger::debugLevel == 8 || Debugger::debugLevel == 9) Debugger::Log("KeyboardControlSystem: Player Right");
                        }
                    }
                }
                // Key Up Logic
                else {
                    if (keyboardcontrol.allowVerticalMovement && (event.key == SDLK_UP || event.key == SDLK_DOWN || event.key == SDLK_w || event.key == SDLK_s)) {
                        rigidbody.velocity.y = 0;
                    }
                    if (keyboardcontrol.allowHorizontalMovement && (event.key == SDLK_LEFT || event.key == SDLK_RIGHT || event.key == SDLK_a || event.key == SDLK_d)) {
                        rigidbody.velocity.x = 0;
                    }
                }
				
			}
		}
};

#endif