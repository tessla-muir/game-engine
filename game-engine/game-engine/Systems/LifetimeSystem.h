#ifndef LIFETIMESYSTEM_H
#define LIFETIMESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/LifetimeComponent.h"
#include "../Debugger/Debugger.h"

class LifetimeSystem : public System {
	public:
		LifetimeSystem() {
			RequireComponent<LifetimeComponent>();
		}

		void Update() {
			// Get relevant entries
			for (auto entity : GetEntities()) {
				LifetimeComponent lifetime = entity.GetComponent<LifetimeComponent>();

				// Destory entity after its lifetime duration has passed
				if (SDL_GetTicks() - lifetime.startTime > lifetime.duration) {
					if (Debugger::debugLevel == 7 || Debugger::debugLevel == 9) Debugger::Log("LifetimeSystem: Entity " + std::to_string(entity.GetId()) + " destroyed after " + std::to_string(lifetime.duration/1000) + " seconds");
					entity.Destroy();
				}
			}
		}
};

#endif