#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../Logger/Logger.h"
#include "../Debugger/Debugger.h"
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ParticleComponent.h"
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
			Entity a = event.a;
			Entity b = event.b;

			if (a.BelongsToGroup("Projectiles") && b.HasTag("Player")) {
				PlayerProjectileInteraction(a, b);
			}
			else if (a.HasTag("Player") && b.BelongsToGroup("Projectiles")) {
				PlayerProjectileInteraction(b, a);
			}
			else if (a.BelongsToGroup("Enemy") && b.BelongsToGroup("Projectiles")) {
				EnemyProjectileInteraction(b, a);
			}
			else if (a.BelongsToGroup("Projectiles") && b.BelongsToGroup("Enemy")) {
				EnemyProjectileInteraction(a, b);
			}
		}

		void PlayerProjectileInteraction(Entity proj, Entity player) {
			ParticleComponent particle = proj.GetComponent<ParticleComponent>();
			
			if (!particle.isFriendly) {
				if (Debugger::debugLevel == 6 || Debugger::debugLevel == 9) Debugger::Log("DamageSystem: Player hit by projectile " + std::to_string(proj.GetId()));
				Logger::Log("PLAYER DIED");

				proj.Destroy();
				player.Destroy();
			}
		}

		void EnemyProjectileInteraction(Entity proj, Entity enemy) {
			ParticleComponent particle = proj.GetComponent<ParticleComponent>();

			if (particle.isFriendly) {
				if (Debugger::debugLevel == 6 || Debugger::debugLevel == 9) Debugger::Log("DamageSystem: Enemy " + std::to_string(enemy.GetId()) + " hit by projectile " + std::to_string(proj.GetId()));
				
				proj.Destroy();
				enemy.Destroy();
			}
		}
};

#endif