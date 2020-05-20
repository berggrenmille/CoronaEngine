
#include "ECS.h"
#include <cassert>
#include <iostream>

namespace Corona
{
	template <typename ... Cs>
	constexpr uint64_t ECS::ComponentMask()
	{
		return ((Factory::TypeId<void>::GetFlag<Cs>()) | ... );
	}

	template <typename C>
	C& ECS::RegisterComponent(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<void>::GetId<C>();
		const uint64_t typeMask = Factory::TypeId<void>::GetFlag<C>();
		assert((entity->componentMask & typeMask) == 0 && "Trying to register an already existing component");
		entity->componentMask |= typeMask;

		//Check if componentArray exists
		if (componentMap.find(typeId) == componentMap.end())
			componentMap[typeId] = new rttr::variant[MAX_ENTITIES]();

		rttr::type t = rttr::type::get<C>();
		//TODO: DEALLOCATE COMPONENT IF NEEDED
		componentMap[typeId][entity->dataIndex] = t.create();


		return componentMap[typeId][entity->dataIndex].get_value<C>();
	}

	template <typename C>
	void ECS::DeleteComponent(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<void>::GetId<C>();
		const uint64_t typeMask = Factory::TypeId<void>::GetFlag<C>();

		entity->componentMask ^= typeMask;
		
		//TODO: DEALLOCATE COMPONENT IF NEEDED
	}

	template <typename C>
	C& ECS::GetComponent(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<void>::GetId<C>();
		const uint64_t typeMask = Factory::TypeId<void>::GetFlag<C>();
		assert((entity->componentMask & typeMask) == typeMask && "Entity does not represent given component");

		return *rttr::detail::unsafe_variant_cast<C>(&(componentMap[typeId][entity->dataIndex]));
	}

	template <typename S>
	S& ECS::RegisterSystem(Entity* entity)
	{
		const int64_t typeId = Factory::TypeId<char>::GetId<S>();
		assert(!systemPool[typeId].is_valid() && "System is already registred");

		//Run time reflection
		rttr::type t = rttr::type::get<S>();
		systemPool[typeId] = t.create();
		return systemPool[typeId].get_value<S>();
	}

	template <typename S>
	void ECS::DeleteSystem(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<char>::GetId<S>();
		assert(systemPool[typeId].is_valid() && "System is not registred");

		systemPool[typeId].clear();
	}

	template <typename S>
	S& ECS::GetSystem(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<char>::GetId<S>();
		assert(systemPool[typeId].is_valid() && "System is not registred");

		return systemPool[typeId].get_value<S>();
	}

	template <typename ... Cs>
	void ECS::ForEach(std::function<void(Cs&...)> function)
	{
		const uint64_t componentMask = ComponentMask<Cs...>();

		for(uint64_t i = 0; i<numAliveEntities; ++i)
		{
			if ((entityPool[i].componentMask & componentMask) == componentMask)
				function(GetComponent<Cs>(&entityPool[i])...);
		}
	}
}
