#include "ECS.h"
#include <vector>
#include "../Logger/Logger.h"

int BaseComponent::nextId = 0;

int Entity::GetId() const {
	return id;
}

void System::AddEntity(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntity(Entity entity) {
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
		return entity == other;
	}), entities.end());
}

std::vector<Entity> System::GetEntities() const {
	return entities;
}

const Signature& System::GetSignature() const {
	return signature;
}

void ComponentManager::Update() {
	// Add entites waiting to be added

	// Remove entites waiting to be removed
}

Entity ComponentManager::CreateEntity() {
	int id;
	id = numEntities++;

	Entity entity(id);
	entitiesToAdd.insert(entity);

	Logger::Log("Entity created with id " + std::to_string(id));

	return entity;
}

