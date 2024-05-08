#ifndef PROJECTILEDISCHARGESYSTEM_H
#define PROJECTILEDISCHARGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Events/EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Components/ProjectileDischargerComponent.h"
#include "../Components/LifetimeComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/ParticleComponent.h"
#include <SDL.h>
#include "../Debugger/Debugger.h"

class ProjectileDischargeSystem : public System {
	public:
		ProjectileDischargeSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<ProjectileDischargerComponent>();
		}

		void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->ListenToEvent<KeyPressedEvent>(this, &ProjectileDischargeSystem::onKeyPressed);
		}

		void onKeyPressed(KeyPressedEvent& event) {
			if (event.key == SDLK_SPACE) {
				for (auto entity : GetEntities()) {
					// Get player -- only player should shoot
					if (entity.HasTag("Player")) {
						ProjectileDischargerComponent& projectileDischarger = entity.GetComponent<ProjectileDischargerComponent>();
						TransformComponent transform = entity.GetComponent<TransformComponent>();
						RigidBodyComponent rigidbody = entity.GetComponent<RigidBodyComponent>();

						// Check that it's been long enough for fire rate
						if (SDL_GetTicks() - projectileDischarger.lastDischargeTime > projectileDischarger.fireRate) {
							// Starting projectile position
							glm::vec2 projPosition = transform.position;
							if (entity.HasComponent<SpriteComponent>()) {
								// Push to center of sprite
								SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
								projPosition.x += (transform.scale.x * sprite.width / 2);
								projPosition.y += (transform.scale.y * sprite.height / 2);
							}

							// Add the projectile
							Entity projectile = entity.compManager->CreateEntity();
							projectile.AddComponent<TransformComponent>(projPosition, glm::vec2(1.0, 1.0), 0.0);
							projectile.AddComponent<RigidBodyComponent>(projectileDischarger.velocity);
							projectile.AddComponent<SpriteComponent>("projectile2", 8, 8);
							projectile.AddComponent<BoxColliderComponent>(8, 8);
							projectile.AddComponent<LifetimeComponent>(projectileDischarger.projectileDuration);
							projectile.AddComponent<ParticleComponent>(true);
							projectile.Group("Projectiles");

							if (Debugger::debugLevel == 4 || Debugger::debugLevel == 9) Debugger::Log("ProjectileDischargeSystem: Player shot out projectile " + std::to_string(projectile.GetId()));

							// Update time
							projectileDischarger.lastDischargeTime = SDL_GetTicks();
						}
					}
				}
			}
		}

		void Update() {
			// Look at the relevant entities
			for (auto entity: GetEntities()) {
				ProjectileDischargerComponent& projectileDischarger = entity.GetComponent<ProjectileDischargerComponent>();
				TransformComponent transform = entity.GetComponent<TransformComponent>();

				// Consisten Fire Frequency
				if (projectileDischarger.dischargeFrequency != 0) {
					if (SDL_GetTicks() - projectileDischarger.lastDischargeTime > projectileDischarger.dischargeFrequency) {
						glm::vec2 projPosition = transform.position;
						if (entity.HasComponent<SpriteComponent>()) {
							// Push to center of sprite
							SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
							projPosition.x += (transform.scale.x * sprite.width / 2);
							projPosition.y += (transform.scale.y * sprite.height / 2);
						}

						// Add the projectile
						Entity projectile = entity.compManager->CreateEntity();
						projectile.AddComponent<TransformComponent>(projPosition, glm::vec2(1.0, 1.0), 0.0);
						projectile.AddComponent<RigidBodyComponent>(projectileDischarger.velocity);
						projectile.AddComponent<SpriteComponent>("projectile1", 8, 8);
						projectile.AddComponent<BoxColliderComponent>(8, 8);
						projectile.AddComponent<LifetimeComponent>(projectileDischarger.projectileDuration);
						projectile.AddComponent<ParticleComponent>(false);
						projectile.Group("Projectiles");

						if (Debugger::debugLevel == 4 || Debugger::debugLevel == 9) Debugger::Log("ProjectileDischargeSystem: Entity " + std::to_string(entity.GetId()) + " shot out projectile " + std::to_string(projectile.GetId()));

						// Update time
						projectileDischarger.lastDischargeTime = SDL_GetTicks();
					}
				}
				// Random Fire Frequency
				else if (projectileDischarger.randomTime != 0) {
					if (SDL_GetTicks() - projectileDischarger.lastDischargeTime > projectileDischarger.randomTime) {
						glm::vec2 projPosition = transform.position;
						if (entity.HasComponent<SpriteComponent>()) {
							// Push to center of sprite
							SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
							projPosition.x += (transform.scale.x * sprite.width / 2);
							projPosition.y += (transform.scale.y * sprite.height / 2);
						}

						// Add the projectile
						Entity projectile = entity.compManager->CreateEntity();
						projectile.AddComponent<TransformComponent>(projPosition, glm::vec2(1.0, 1.0), 0.0);
						projectile.AddComponent<RigidBodyComponent>(projectileDischarger.velocity);
						projectile.AddComponent<SpriteComponent>("projectile1", 8, 8);
						projectile.AddComponent<BoxColliderComponent>(8, 8);
						projectile.AddComponent<LifetimeComponent>(projectileDischarger.projectileDuration);
						projectile.AddComponent<ParticleComponent>(false);
						projectile.Group("Projectiles");

						if (Debugger::debugLevel == 4 || Debugger::debugLevel == 9) Debugger::Log("ProjectileDischargeSystem: Entity " + std::to_string(entity.GetId()) + " shot out projectile " + std::to_string(projectile.GetId()));

						// Update time
						projectileDischarger.lastDischargeTime = SDL_GetTicks();

						// Get new random number
						projectileDischarger.min = 5000;
						projectileDischarger.randomTime = projectileDischarger.min + rand() % (projectileDischarger.max - projectileDischarger.min + 1);
					}
				}
			}
		}
};

#endif