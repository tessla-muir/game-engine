#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include <glm.hpp>

class CollisionSystem : public System {
	public:
		CollisionSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<BoxColliderComponent>();
		}

		void Update(std::unique_ptr<EventBus>& eventBus) {
			auto entities = GetEntities();

			// Go through all the relevant entities
			for (auto i = entities.begin(); i != entities.end(); i++) {
				Entity one = *i;

				for (auto j = i; j != entities.end(); j++) {
					Entity two = *j;

					if (one == two) continue; // Don't compare a collider to itself

					TransformComponent oneTransf = one.GetComponent<TransformComponent>();
					BoxColliderComponent oneCollid = one.GetComponent<BoxColliderComponent>();
					TransformComponent twoTransf = two.GetComponent<TransformComponent>();
					BoxColliderComponent twoCollid = two.GetComponent<BoxColliderComponent>();

					// AABB Collision
					if (CheckCollision(
						oneTransf.position.x + oneCollid.offset.x,
						oneTransf.position.y + oneCollid.offset.y,
						oneCollid.width,
						oneCollid.height,
						twoTransf.position.x + twoCollid.offset.x,
						twoTransf.position.y + twoCollid.offset.y,
						twoCollid.width,
						twoCollid.height
					)) {
						// Collision!
						eventBus->DispatchEvent<CollisionEvent>(one, two);
					}
				}
			}
		}

		// AABB Collision
		bool CheckCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
			return (
				aX < bX + bW &&
				aX + aW > bX &&
				aY < bY + bH &&
				aY + aH > bY
			);
		}
};

#endif