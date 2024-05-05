#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map> 
#include <typeindex>
#include <set>
#include "../Logger/Logger.h"

// Signature to keep track of what components an entity has
typedef std::bitset<32> Signature;

struct BaseComponent {
	protected:
		static int nextId;
};

template <typename T>
class Component: public BaseComponent {
	static int GetId() {
		static auto id = nextId;
		return id;
	}
};

class Entity {
	private:
		int id;

	public:
		Entity(int id) : id(id) {};
		int GetId() const;
		
		Entity& operator =(const Entity& entitiy) = default;
		bool operator ==(const Entity& entity) const { return id == entity.id; }
		bool operator !=(const Entity& entity) const { return id != entity.id; }
		bool operator <(const Entity& entity) const { return id < entity.id; }
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

		template <typename T> void RequireComponent();
};

class BasePool {
	public:
		virtual ~BasePool() {}
};

// A wrapper class for a vector
template <typename T>
class Pool : public BasePool {
private:
	std::vector<T> pool;

public:
	Pool(int size = 100) { Resize(size); }
	~Pool() = default;

	bool IsEmpty() const { return pool.empty(); }
	int GetSize() const { return pool.size(); }
	void Resize(int size) { pool.resize(size); }
	void Clear() { pool.clear(); }
	void Add(T object) { pool.push_back(object); }
	void Set(int index, T object) { pool[index] = object; }
	T& Get(int index) { return static_cast<T&>(pool[index]); }
	T& operator [](unsigned int index) { return pool[index]; }
};

class ComponentManager {
	private:
		int numEntities = 0;
		std::set<Entity> entitiesToAdd;
		std::set<Entity> entitiesToRemove;

		// Vector of component pools, each contains all data for a component type
		// Vector index is the component type id
		std::vector<BasePool*> componentPools;

		// Vector of component signatures, shows which components are used/on for an entitiy
		// Vector index is the entity id
		std::vector<Signature> entitySignatures;

		// Map of all the active systems
		// Index is the system typeid
		std::unordered_map<std::type_index, System*> systems;

	public:
		ComponentManager() = default;

		void Update(); // Waits until frame end to update entites (add/remove)

		Entity CreateEntity();
		//void RemoveEntity(Entity entity);

		template <typename T> bool HasComponent(Entity entity);
		template <typename T, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		template <typename T> void RemoveComponent(Entity entity);

		//bool HasSystem(System system);
		//void AddSystem(System system);
		//void RemoveSystem(System system);
};

template <typename T>
void System::RequireComponent() {
	const auto componentId = Component<T>::GetId();
	signature._Set_unchecked(componentId);
}

template <typename T>
bool ComponentManager::HasComponent(Entity entity) {
	// Get component and entity IDs
	const int componentId = Component<T>.GetId();
	const int entityId = entity.GetId();

	return entitySignatures[entityId].test(componentId);
}

template <typename T, typename ...TArgs>
void ComponentManager::AddComponent(Entity entity, TArgs&& ...args) {
	// Get component and entity IDs
	const int componentId = Component<T>.GetId();
	const int entityId = entity.GetId();

	// Ensure the component id is in componentPull size, adjust size if not
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	// Create pool for component type if needed
	if (!componentPools[componentId]) {
		Pool<T>* newPool = new Pool<T>();
		componentPools[componentId] = newPool;
	}

	// Get pool that matches component type (sprite, transform, etc.)
	Pool<T>* pool = Pool<T>(componentPools[componentId]);

	// Ensure the entity id is in the individual component pool, adjust size if not
	if (entityId >= pool->GetSize()) {
		pool->Resize(numEntities);
	}

	// Create the new component to add, passing arguments
	T newComp(std::forward<TArgs>(args)...);
	pool->Set(entityId, newComp);

	// Set new signature
	entitySignatures[entityId].set(componentId);
}

template <typename T>
void ComponentManager::RemoveComponent(Entity entity) {
	// Get component and entity IDs
	const int componentId = Component<T>.GetId();
	const int entityId = entity.GetId();

	if (!HasComponent<T>(entity)) {
		Logger::Warn("ECS.h: Entity " + std::to_string(entityId) + " didn't have component " + std::to_string(componentId) + " to remove.");
		return;
	}

	// TODO: Deallocate

	// Deactivate signature part
	entitySignatures[entityId].set(componentId, false);
}

#endif 