#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ScoreComponent.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events//CollisionEvent.h"
#include "../Logger/Logger.h"
#include <memory>

class ScoreSystem : public System {
	private:
		int score = 0;

	public:
		ScoreSystem() {
			RequireComponent<ScoreComponent>();
		}

		void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->ListenToEvent<CollisionEvent>(this, &ScoreSystem::onCollision);
		}

		void onCollision(CollisionEvent& event) {
			Entity a = event.a;
			Entity b = event.b;

			if (a.BelongsToGroup("Enemy") && b.BelongsToGroup("Projectiles")) {
				EnemyProjectileInteraction(b, a);
			}
			else if (a.BelongsToGroup("Projectiles") && b.BelongsToGroup("Enemy")) {
				EnemyProjectileInteraction(a, b);
			}
		}

		void EnemyProjectileInteraction(Entity proj, Entity enemy) {
			ParticleComponent particle = proj.GetComponent<ParticleComponent>();

			if (particle.isFriendly) {
				if (!enemy.HasComponent<ScoreComponent>()) return;

				// Add to score
				ScoreComponent scoreComp = enemy.GetComponent<ScoreComponent>();
				score += scoreComp.score;
				Logger::Log("Score: " + std::to_string(score));
			}
		}
};

#endif