#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

#define MAX_COMPONENTS 32

// A component signature is a bitset of 32 bits and it 
// represents which components an entity belongs to.
// Each component holds a specific bit position in the bitset.
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int nextId;
};

template<typename T>
class Component : public IComponent {
	// TODO:
public:
	const static int GetId() {
		static constexpr auto id = nextId++;
		return id;
	}
};

class Entity {
	// TODO:
private:
	int id;
public:
	Entity(int id) : id(id) {}
	int GetId() const {
		return id;
	}

	Entity& operator =(const Entity& other) = default;

	bool operator ==(const Entity& b) const {
		return id == b.id;
	}
	
	bool operator !=(const Entity& b) const {
		return id != b.id;
	}
	
	bool operator >(const Entity& b) const {
		return id > b.id;
	}

	bool operator <(const Entity& b) const {
		return id < b.id;
	}
};


// A System processes entities that has a certain signature.
class System {
	// TODO:
private:
	Signature componentSignature;
	std::vector<Entity> entities;

public:
	System() = default;
	virtual ~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> getEntities() const;
	const Signature& GetComponentSignature() const;

	// Define the component type T that entities must have to be
	// considered by the system
	template<typename TComponent> 
	void RequireComponent();
};

template<typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

class IPool {
public:
	virtual ~IPool() = default;
};

//A pool is going to hold the entity component data per entity 
template<typename T>
class Pool : public IPool {
private:
	std::vector<T> data;
public:
	Pool(size_t size = 100) { data.resize(size); };

	virtual ~Pool() = default;

	void Resize(int size = 100) {
		data.resize(size);
	}

	size_t Size() {
		return data.size();
	}

	void Set(size_t index, T & typeData) {
		data[index] = typeData;
	}

	T& Get(size_t index) {
		return static_cast<T&>(data[index]);
	}

	T& operator[](size_t index) {
		return data[index];
	}

	bool IsEmpty() const {
		return data.empty();
	}
};

// Registry will be responsible for creating, managing and destroying
// Entities, Components and Systems.
class Registry {
private:
	// Keep track of how many entities there are in total.
	int countEntities = 0;
	std::set<Entity> entitiesToBeAdded; // Entities awaiting creation in the next Registry Update()
	std::set<Entity> entitiesToBeKilled; // Entities awaiting destruction in the next Registry Update()

	// Vector of pools. Each pool represents the entity-wise component data.
	// Each component type will have its own pool.
	std::vector<IPool*> componentPools;

	// Vector of component signatures.
	// The signature lets us know which components are turned "on" for an entity
	// [vector index = entity id]
	std::vector<Signature> entityComponentSignatures;

	// Map of active systems [index = system typeid]
	std::unordered_map<std::type_index, System*> systems;


public:
	Registry() = default;

	// Management of Entities
	Entity CreateEntity();
	void KillEntity(Entity entity);
	
	
	template<typename ComponentType, typename ... ConstructorArgs>
	void AddComponent(Entity entity, ConstructorArgs&& ... args) {
		//Get the IDs
		const auto componentId = Component<ComponentType>::GetId();
		const auto entityId = entity.GetId();

		// TODO: Set the component in the corresponding pool
		//Resize component pool if the componentId is greater than the number of pools
		if (componentId >= componentPools.size()) {
			componentPools.resize(componentId + 1);
		}

		//If the component pool is not present, create the pool for the component type and add it to the vector
		if (!componentPools[componentId]) {
			IPool* componentPool = new Pool<ComponentType>();
			componentPools[componentId] = componentPool;
		}

		//Get pool for component type
		Pool<ComponentType>* pool = Pool<ComponentType>(componentPools[componentId]);
		ComponentType component(std::forward<ConstructorArgs>(args)...);

		//Resize pool if entity id greater than pool size
		if (entityId >= pool->Size()) {
			pool->Resize(countEntities);
		}

		//Add the component data to the pool using entity id as the index
		pool->Set(entityId, component);
		
		//Update the component signature
		entityComponentSignatures[entityId].set(componentId, true);
	}
	 
	template<typename ComponentType>
	void RemoveComponent(Entity entity) {
		const auto componentId = Component<ComponentType>::GetId();
		const auto entityId = entity.GetId();

		entityComponentSignatures[entityId].set(componentId, false);
	}
	
	template<typename ComponentType>
	void HasComponent(Entity entity) {
		const auto componentId = Component<ComponentType>::GetId();
		const auto entityId = entity.GetId();
		return entityComponentSignatures[entityId].test(componentId);
	}


	template<typename T, typename ... Args>
	void AddSystem(Args&&...args) {
		T* system = new T(std::forward<Args>(args)...);
		systems.insert(std::make_pair<>( std::type_index(T), system));
	}

	template<typename T>
	void RemoveSystem() {
		if (systems.find(std::type_index(T)) != systems.end()) {
			T* system = systems.at(std::type_index(T));
			delete system;
			systems.erase(std::type_index(T));
		}
	}

	template<typename T>
	T& GetSystem() {
		if (systems.find(std::type_index(T)) != systems.end()) {
			return *std::static_pointer_cast<T>(systems.at(std::type_index(T)));
		}

		return null;
	}

	template<typename T>
	bool HasSystem() {
		return systems.find(std::type_index(T)) != systems.end();
	}


	void AddEntityToSystems(Entity entity);
	void Update();

};