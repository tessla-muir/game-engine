#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map> 
#include <typeindex>
#include <set>
#include <deque>
#include <memory>
#include "../Logger/Logger.h"
#include <glm.hpp>

// Signature to keep track of what components an entity has
typedef std::bitset<32> Signature;

struct BaseComponent {
	protected:
		static int nextId;
};

template <typename TComp>
class Component: public BaseComponent {
	public:
		static int GetId() {
			static auto id = nextId++;
			return id;
		}
};

class ComponentManager; // Forward declaration for Entity

class Entity {
	private:
		int id;

	public:
		Entity() = default;
		Entity(int id) : id(id) {};
		Entity(const Entity& entity) = default;
		int GetId() const;
		ComponentManager* compManager;
		void Destroy();
		
		Entity& operator =(const Entity& entitiy) = default;
		bool operator ==(const Entity& entity) const { return id == entity.id; }
		bool operator !=(const Entity& entity) const { return id != entity.id; }
		bool operator <(const Entity& entity) const { return id < entity.id; }

		template <typename TComp> bool HasComponent() const;
		template <typename TComp> TComp& GetComponent() const;
		template <typename TComp, typename ...TArgs> void AddComponent(TArgs&& ...args);
		template <typename TComp> void RemoveComponent();
};

class System {
	private:
		Signature signature;
		std::vector<Entity> entities;

	public:
		System() = default;
		~System() = default;
		void AddEntity(Entity entity);
		void RemoveEntity(Entity entity);
		std::vector<Entity> GetEntities() const;
		const Signature& GetSignature() const;

		template <typename TComp> void RequireComponent();
};

class BasePool {
	public:
		virtual ~BasePool() {}
};

// A wrapper class for a vector
template <typename TComp>
class Pool : public BasePool {
private:
	std::vector<TComp> pool;

public:
	Pool(int size = 100) { Resize(size); }
	~Pool() = default;

	bool IsEmpty() const { return pool.empty(); }
	int GetSize() const { return pool.size(); }
	void Resize(int size) { pool.resize(size); }
	void Clear() { pool.clear(); }
	void Add(TComp object) { pool.push_back(object); }
	void Set(int index, TComp object) { pool[index] = object; }
	TComp& Get(int index) { return static_cast<TComp&>(pool[index]); }
	TComp& operator [](unsigned int index) { return pool[index]; }
};

class ComponentManager {
	private:
		int numEntities = 0;
		std::set<Entity> entitiesToAdd;
		std::set<Entity> entitiesToRemove;

		// Vector of component pools, each contains all data for a component type
		// Vector index is the component type id
		std::vector<std::shared_ptr<BasePool>> componentPools;

		// Vector of component signatures, shows which components are used/on for an entitiy
		// Vector index is the entity id
		std::vector<Signature> entitySignatures;

		// Map of all the active systems
		// Index is the system typeid
		std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

		// Holds freed ids
		std::deque<int> availableIds;

	public:
		ComponentManager() = default;

		void Update(); // Waits until frame end to update entites (add/remove)

		// Entity Management
		Entity CreateEntity();
		void RemoveEntity(Entity entity);
		void AddEntityToSystems(Entity entity);
		void RemoveEntityFromSystems(Entity entity);

		// Component Management
		template <typename TComp> bool HasComponent(Entity entity) const;
		template <typename TComp> TComp& GetComponent(Entity entity) const;
		template <typename TComp, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		template <typename TComp> void RemoveComponent(Entity entity);

		// System Management
		template <typename TSys> bool HasSystem() const;
		template <typename TSys, typename ...TArgs> void AddSystem(TArgs&& ...args);
		template <typename TSys> void RemoveSystem();
		template <typename TSys> TSys& GetSystem() const;
};

template <typename TComp>
bool Entity::HasComponent() const {
	return compManager->HasComponent<TComp>(*this);
}

template <typename TComp>
TComp& Entity::GetComponent() const {
	return compManager->GetComponent<TComp>(*this);
}

template <typename TComp, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
	compManager->AddComponent<TComp>(*this, std::forward<TArgs>(args)...);
}

template <typename TComp>
void Entity::RemoveComponent() {
	compManager->RemoveComponent<TComp>(*this);
}

template <typename TComp>
bool ComponentManager::HasComponent(Entity entity) const {
	// Get component and entity IDs
	const int componentId = Component<TComp>::GetId();
	const int entityId = entity.GetId();

	return entitySignatures[entityId].test(componentId);
}

template <typename TComp> 
TComp& ComponentManager::GetComponent(Entity entity) const {
	// Get component and entity IDs
	const int componentId = Component<TComp>::GetId();
	const int entityId = entity.GetId();

	std::shared_ptr<Pool<TComp>> pool = std::static_pointer_cast<Pool<TComp>>(componentPools[componentId]);
	return pool->Get(entityId);
}

template <typename TComp, typename ...TArgs>
void ComponentManager::AddComponent(Entity entity, TArgs&& ...args) {
	// Get component and entity IDs
	const int componentId = Component<TComp>::GetId();
	const int entityId = entity.GetId();

	// Ensure the component id is in componentPull size, adjust size if not
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	// Create pool for component type if needed
	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComp>> newPool = std::make_shared<Pool<TComp>>();
		componentPools[componentId] = newPool;
	}

	// Get pool that matches component type (sprite, transform, etc.)
    std::shared_ptr<Pool<TComp>> pool = std::static_pointer_cast<Pool<TComp>>(componentPools[componentId]);

	// Create the new component to add, passing arguments
	TComp newComp(std::forward<TArgs>(args)...);
	pool->Set(entityId, newComp);

	// Set new signature
	entitySignatures[entityId].set(componentId);

	Logger::Log("Component " + std::to_string(componentId) + " added to entity " + std::to_string(entityId));
}

template <typename TComp>
void ComponentManager::RemoveComponent(Entity entity) {
	// Get component and entity IDs
	const int componentId = Component<TComp>::GetId();
	const int entityId = entity.GetId();

	if (!HasComponent<TComp>(entity)) {
		Logger::Warn("ECS.h: Entity " + std::to_string(entityId) + " didn't have component " + std::to_string(componentId) + " to remove.");
		return;
	}

	//// Remove component
	//std::shared_ptr<Pool<TComp>> compPool = std::static_pointer_cast<Pool<TComp>>(componentPools[componentId]);
	//compPool->Remove(entityId);

	// Deactivate signature part
	entitySignatures[entityId].set(componentId, false);

	Logger::Log("Component " + std::to_string(componentId) + " removed from entity " + std::to_string(entityId));
}

template <typename TComp>
void System::RequireComponent() {
	int componentId = Component<TComp>::GetId();
	signature.set(componentId);
}

template <typename TSys> 
bool ComponentManager::HasSystem() const {
	return systems.find(std::type_index(typeid(TSys))) != system.end();
}

template <typename TSys, typename ...TArgs>
void ComponentManager::AddSystem(TArgs&& ...args) {
	// Create component
	std::shared_ptr<TSys> newSys = std::make_shared<TSys>(std::forward<TArgs>(args)...);

	// Add to map
	systems.insert(std::make_pair(std::type_index(typeid(TSys)), newSys));
}

template <typename TSys>
void ComponentManager::RemoveSystem() {
	// Find the system to remove
	auto system = systems.find(std::type_index(typeid(TSys)));

	// Remove system
	systems.erase(system);
}

template <typename TSys> 
TSys& ComponentManager::GetSystem() const {
	// Find & return the system
	auto system = systems.find(std::type_index(typeid(TSys)));
	return *std::static_pointer_cast<TSys>(system->second);
}

#endif 