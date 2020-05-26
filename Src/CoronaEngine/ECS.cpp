#include "ECS.h"


namespace Corona
{
	uint64_t ECS::GetDataIndex()
	{
		uint64_t index;
		if(freeDataIndices.empty())
		{
			while (reservedDataIndices.find(dataIndexCounter) != reservedDataIndices.end())
				dataIndexCounter++;
			return dataIndexCounter;
		}
		else
		{
			index = freeDataIndices.front();
			freeDataIndices.pop();
		}
		return index;
	}

	void ECS::FreeDataIndex(uint64_t index)
	{
		freeDataIndices.push(index);
		if (reservedDataIndices.find(index) != reservedDataIndices.end())
			reservedDataIndices.erase(index);
	}

	uint64_t ECS::ReserveDataIndex(uint64_t index)
	{
		assert(reservedDataIndices.find(index) == reservedDataIndices.end() && "DataIndex already reserved!");
		reservedDataIndices.insert(index);
		return index;
	}

	void ECS::Refresh()
	{
		int64_t dead = 0, alive = numAliveEntities;
		while(true)
		{
			for(;true;++dead)
			{
				if (dead >= alive)
				{
					return;
				}

				if (!entityVector[dead].alive)
				{
					numAliveEntities--;
					break; // Found dead entity
				}
			}

			for (; true; --alive)
			{
				if (entityVector[alive].alive)
					break; // Found alive
				if (alive <= dead)
					return;
			}

			assert(entityVector[alive].alive);
			assert(!entityVector[dead].alive);			
			FreeDataIndex(entityVector[dead].dataIndex);
			entityVector[dead].alive = entityVector[alive].alive;
			entityVector[dead].dataIndex = entityVector[alive].dataIndex;
			entityVector[dead].componentMask = entityVector[alive].componentMask;

			entityVector[alive].alive = false;
			entityVector[alive].componentMask = 0;
			
			++dead;
			--alive;
		
		}
	}

	int64_t ECS::GetNumAliveEntities() const
	{
		return numAliveEntities;
	}

	ECS::Entity* ECS::RegisterEntity()
	{
		if(numAliveEntities == reservedEntities)
		{
			auto newSize = reservedEntities * 2;
			//Allocate more space
			entityVector.resize(newSize);
			componentVector.resize(newSize * MAX_COMPONENTS_PER_ENTITY);
			entityDataIndexToIndex.resize(newSize);
			reservedEntities = newSize;
		}


		const auto dataIndex = GetDataIndex();
		
		
		entityVector[numAliveEntities].componentMask = 0;
		entityVector[numAliveEntities].alive = true;
		entityVector[numAliveEntities].dataIndex = dataIndex;
		entityDataIndexToIndex[dataIndex] = numAliveEntities;

		return &entityVector[numAliveEntities++];
	}

	void ECS::DeleteEntity(Entity* entity)
	{
		entity->alive = false;
	}
}
