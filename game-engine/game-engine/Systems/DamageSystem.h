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
			//Logger::Debug("DamageSystem.cs: Recieve Collision Event between " + std::to_string(event.a.GetId()) + " and " + std::to_string(event.b.GetId()));
			//event.a.Destroy();
			//event.b.Destroy();
		}

		void Update() {

		}
};

#endif