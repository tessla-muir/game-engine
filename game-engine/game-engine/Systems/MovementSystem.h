#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../Logger/Logger.h"
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

				// Avoid out of boundaries
				if (!entity.BelongsToGroup("Projectiles") && entity.HasComponent<SpriteComponent>()) {
					SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
					int padding = 10;
					transform.position.x = transform.position.x < padding ? padding : transform.position.x;
					transform.position.x = transform.position.x > WIN_WIDTH - padding - sprite.width ? WIN_WIDTH - padding - sprite.width : transform.position.x;
				}
			}
		}
};

# endif