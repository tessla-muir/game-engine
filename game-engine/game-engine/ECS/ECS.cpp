#include "ECS.h"

int BaseComponent::nextId = 0;

int Entity::GetId() const {
	return id;
}

void Entity::Destroy() {
	compManager->RemoveEntity(*this);
}

void Entity::Tag(const std::string& tag) {
	compManager->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const {
	return compManager->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& group) {
	compManager->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group) const {
	return compManager->EntityBelongsToGroup(*this, group);
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

	if (Debugger::debugLevel == 1 || Debugger::debugLevel == 2 || Debugger::debugLevel == 9) Debugger::Log("ECS: Entity created with id " + std::to_string(id));

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
	if (Debugger::debugLevel == 1 || Debugger::debugLevel == 2 || Debugger::debugLevel == 9) Debugger::Log("ECS: Entity removed with id " + std::to_string(entity.GetId()));
}

void ComponentManager::RemoveEntityFromSystems(Entity entity) {
	for (auto system : systems) {
		system.second->RemoveEntity(entity);
	}
}

void ComponentManager::TagEntity(Entity entity, const std::string& tag) {
	entityPerTag.emplace(tag, entity);
	tagPerEntity.emplace(entity.GetId(), tag);
}

bool ComponentManager::EntityHasTag(Entity entity, const std::string& tag) const {
	if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end()) {
		return false;
	}
	return entityPerTag.find(tag)->second == entity;
}

Entity ComponentManager::GetEntityByTag(const std::string& tag) const {
	return entityPerTag.at(tag);
}

void ComponentManager::RemoveEntityTag(Entity entity) {
	auto taggedEntity = tagPerEntity.find(entity.GetId());
	if (taggedEntity != tagPerEntity.end()) {
		auto tag = taggedEntity->second;
		entityPerTag.erase(tag);
		tagPerEntity.erase(taggedEntity);
	}
}

void ComponentManager::GroupEntity(Entity entity, const std::string& group) {
	entitiesPerGroup.emplace(group, std::set<Entity>());
	entitiesPerGroup[group].emplace(entity);
	groupPerEntity.emplace(entity.GetId(), group);
}

bool ComponentManager::EntityBelongsToGroup(Entity entity, const std::string& group) const {
	if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
		return false;
	}
	auto groupEntities = entitiesPerGroup.at(group);
	return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> ComponentManager::GetEntitiesByGroup(const std::string& group) const {
	auto& setOfEntities = entitiesPerGroup.at(group);
	return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void ComponentManager::RemoveEntityGroup(Entity entity) {
	// if in group, remove entity from group management
	auto groupedEntity = groupPerEntity.find(entity.GetId());
	if (groupedEntity != groupPerEntity.end()) {
		auto group = entitiesPerGroup.find(groupedEntity->second);
		if (group != entitiesPerGroup.end()) {
			auto entityInGroup = group->second.find(entity);
			if (entityInGroup != group->second.end()) {
				group->second.erase(entityInGroup);
			}
		}
		groupPerEntity.erase(groupedEntity);
	}
}