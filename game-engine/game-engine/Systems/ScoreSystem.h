#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ScoreComponent.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events//CollisionEvent.h"
#include "../Logger/Logger.h"
#include <memory>

class ScoreSystem : public System {
	public:
		ScoreSystem() {
			RequireComponent<ScoreComponent>();
		}

		void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->ListenToEvent<ScoreChangeEvent>(this, &ScoreSystem::onScoreChange);
		}

		void onScoreChange(ScoreChangeEvent& event) {
			playerEntity.GetComponent<ScoreComponent>().score += event.score;
			Logger::Log("Score: " + std::to_string(playerEntity.GetComponent<ScoreComponent>().score));
		}

		void SetPlayerEntity(Entity entity) {
			playerEntity = entity;
		}

	private:
		Entity playerEntity;
};

#endif