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
	for (Entity entity : entitiesToAdd) {
		AddEntityToSystems(entity);
	}
	entitiesToAdd.clear(); // Clear for next frame

	// TODO: Remove entites waiting to be removed
}

Entity ComponentManager::CreateEntity() {
	int id;
	id = numEntities++;

	Entity entity(id);
	entity.compManager = this;
	entitiesToAdd.insert(entity);

	if (id >= entitySignatures.size()) {
		entitySignatures.resize(id + 1);
	}

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