#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>
#include <bitset>
int IComponent::nextId = 0;

void System::AddEntityToSystem(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), 
		entities.end(), 
		[&entity](Entity e) {
			return entity == e;
		}
	),entities.end());
}

std::vector<Entity> System::GetEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}

// Management of Entities
Entity Registry::CreateEntity() {
	int entityId = 0;
	if (freeIds.empty())
		entityId = ++countEntities;
	else {
		entityId = freeIds.front();
		freeIds.pop_front();
	}
	if (entityId >= entityComponentSignatures.size()) {
		entityComponentSignatures.resize(entityId + 1);
	}
	Entity entity(entityId);
	entitiesToBeAdded.insert(entity);
	
	Logger::Log("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::KillEntity(Entity entity)
{
	entitiesToBeKilled.insert(entity);
}

void Registry::AddEntityToSystems(Entity entity)
{
	const auto entityId = entity.GetId();
	
	const auto signature = entityComponentSignatures[entityId];
	
	for (auto [_,system] : systems) {
		if (system->GetComponentSignature() == signature) {
			system->AddEntityToSystem(entity);
		}
	}
}

void Registry::Update(float deltaTime)
{
	// The actual creation and destruction of entities will happen in the update
	// loop. We don't want to create or destroy entities in the middle of the frame logic.
	// So we wait until the end of the frame to update and perform creation and 
	// deletion of entities.
	for (Entity entity : entitiesToBeAdded) {
		for (auto [_, system] : systems) {
			auto systemSignature = system->GetComponentSignature();
			auto entitySignature = entityComponentSignatures[entity.GetId()];
			bool isMatch = (systemSignature & entitySignature) == systemSignature;
			if (isMatch) {
				system->AddEntityToSystem(entity);
			}
		}
	}
	entitiesToBeAdded.clear();
	
	for (Entity entity : entitiesToBeKilled) {
		for (auto [_, system] : systems) {
			auto systemSignature = system->GetComponentSignature();
			auto entitySignature = entityComponentSignatures[entity.GetId()];
			bool isMatch = (systemSignature & entitySignature) == systemSignature;
			if (isMatch) {
				system->RemoveEntityFromSystem(entity);
			}
		}
		entityComponentSignatures[entity.GetId()].reset();
		freeIds.push_back(entity.GetId());
	}
	entitiesToBeKilled.clear();
}
