#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

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

class ComponentManager {

};

template <typename T>
void System::RequireComponent() {
	const auto componentId = Component<T>::GetId();
	signature._Set_unchecked(componentId);
}

#endif 