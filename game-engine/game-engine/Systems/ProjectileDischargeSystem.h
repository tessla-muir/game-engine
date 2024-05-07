#ifndef PROJECTILEDISCHARGESYSTEM_H
#define PROJECTILEDISCHARGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileDischargerComponent.h"
#include "../Components/LifetimeComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>

class ProjectileDischargeSystem : public System {
	public:
		ProjectileDischargeSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<ProjectileDischargerComponent>();
		}

		void Update(std::unique_ptr<ComponentManager>& compManager) {
			// Look at the relevant entities
			for (auto entity: GetEntities()) {
				ProjectileDischargerComponent& projectileDischarger = entity.GetComponent<ProjectileDischargerComponent>();
				TransformComponent transform = entity.GetComponent<TransformComponent>();

				if (SDL_GetTicks() - projectileDischarger.lastDischargeTime > projectileDischarger.dischargeFrequency) {
					glm::vec2 projPosition = transform.position;
					if (entity.HasComponent<SpriteComponent>()) {
						// Push to center of sprite
						SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
						projPosition.x += (transform.scale.x * sprite.width / 2);
						projPosition.y += (transform.scale.y * sprite.height / 2);
					}

					// Add the projectile
					Entity projectile = compManager->CreateEntity();
					projectile.AddComponent<TransformComponent>(projPosition, glm::vec2(1.0, 1.0), 0.0);
					projectile.AddComponent<RigidBodyComponent>(projectileDischarger.velocity);
					projectile.AddComponent<SpriteComponent>("projectile1", 8, 8);
					projectile.AddComponent<BoxColliderComponent>(8, 8);
					projectile.AddComponent<LifetimeComponent>(projectileDischarger.projectileDuration);

					// Update time
					projectileDischarger.lastDischargeTime = SDL_GetTicks();
				}
			}
		}
};

#endif