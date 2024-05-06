#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include"../Components/RigidbodyComponent.h"
#include"../Components/TransformComponent.h"

class MovementSystem : public System {
	public:
		MovementSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
		}

		void Update(float deltaTime) {
			// Find all the relevant entities
			for (auto entity : GetEntities()) {
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				const RigidBodyComponent& rigidbody = entity.GetComponent<RigidBodyComponent>();

				transform.position.x += rigidbody.velocity.x * deltaTime;
				transform.position.y += rigidbody.velocity.y * deltaTime;
			}
		}
};

# endif