#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events//CollisionEvent.h"

class DamageSystem : public System {
	public:
		DamageSystem() {
			RequireComponent<BoxColliderComponent>();
		}

		void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->ListenToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
		}

		void onCollision(CollisionEvent& event) {
			
		}
};

#endif