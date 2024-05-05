#include "ECS.h"
#include <vector>
#include "../Logger/Logger.h"

int BaseComponent::nextId = 0;

int Entity::GetId() const {
	return id;
}

void Entity::Destroy() {
	compManager->RemoveEntity(*this);
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
	for (Entity entity : entitiesToAdd) {
		AddEntityToSystems(entity);
	}
	entitiesToAdd.clear(); // Clear for next frame

	// Remove entites waiting to be removed
	for (Entity entity : entitiesToRemove) {
		RemoveEntityFromSystems(entity);
		entitySignatures[entity.GetId()].reset();

		// Allow the id to be reused
		availableIds.push_back(entity.GetId());
	}
	entitiesToRemove.clear();
}

Entity ComponentManager::CreateEntity() {
	int id;

	if (availableIds.empty()) {
		// Happens if not ids are available to be reused
		id = numEntities++;
		if (id >= entitySignatures.size()) {
			entitySignatures.resize(id + 1);
		}
	} else {
		// Reuse an id if available
		id = availableIds.front();
		availableIds.pop_front();
	}

	Entity entity(id);
	entity.compManager = this;
	entitiesToAdd.insert(entity);

	Logger::Log("Entity created with id " + std::to_string(id));

	return entity;
}

void ComponentManager::AddEntityToSystems(Entity entity) {
	const int entityId = entity.GetId();
	const Signature& entitySignature = entitySignatures[entityId];

	// Look through all systems
	for (auto& system : systems) {
		const Signature& systemSignature = system.second->GetSignature();

		// Add entity to system if wanted
		if ((entitySignature & systemSignature) == systemSignature) {
			system.second->AddEntity(entity);
		}
	}
}

void ComponentManager::RemoveEntity(Entity entity) {
	entitiesToRemove.insert(entity);
	Logger::Log("Entity removed with id " + std::to_string(entity.GetId()));
}

void ComponentManager::RemoveEntityFromSystems(Entity entity) {
	for (auto system : systems) {
		system.second->RemoveEntity(entity);
	}
}