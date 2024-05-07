#ifndef LIFETIMESYSTEM_H
#define LIFETIMESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/LifetimeComponent.h"

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
					entity.Destroy();
				}
			}
		}
};

#endif