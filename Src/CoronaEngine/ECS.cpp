#include "ECS.h"


namespace Corona
{

	std::array<ECS::Entity, MAX_ENTITIES>				ECS::entityPool;
	std::array<rttr::variant, MAX_SYSTEMS>				ECS::systemPool;
	std::unordered_map<uint64_t, rttr::variant*>		ECS::componentMap;

	int64_t ECS::numAliveEntities = 0;

	int64_t ECS::GetNumAliveEntities()
	{
		return numAliveEntities;
	}

	ECS::Entity* ECS::RegisterEntity()
	{
		const uint64_t entityIndex = Factory::Id<Entity>::GetId();
		assert(numAliveEntities < MAX_ENTITIES && "Entity capacity reached!");

		entityPool[entityIndex].componentMask = 0;
		entityPool[entityIndex].index = entityIndex;
		entityPool[entityIndex].isAlive = true;
		entityPool[entityIndex].dataIndex = Factory::DataIndex::GetIndex();

		++numAliveEntities;

		return &entityPool[entityIndex];
	}

	void ECS::DeleteEntity(Entity* entity)
	{
		entity->pendingDeath = true;
	}
}
