
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

		const auto componentIndex = entity->dataIndex * MAX_COMPONENTS_PER_ENTITY + typeId;


		rttr::type t = rttr::type::get<C>();
		componentVector[componentIndex] = t.create();
		return componentVector[componentIndex].get_value<C>();
	}

	template <typename C>
	void ECS::DeleteComponent(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<void>::GetId<C>();
		const uint64_t typeMask = Factory::TypeId<void>::GetFlag<C>();
		const auto componentIndex = entity->dataIndex * MAX_COMPONENTS_PER_ENTITY + typeId;
		entity->componentMask ^= typeMask;

		componentVector[componentIndex].clear();
	}

	template <typename C>
	C& ECS::GetComponent(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<void>::GetId<C>();
		const uint64_t typeMask = Factory::TypeId<void>::GetFlag<C>();
		assert((entity->componentMask & typeMask) == typeMask && "Entity does not represent given component");
		const auto componentIndex = entity->dataIndex * MAX_COMPONENTS_PER_ENTITY + typeId;

		return *rttr::detail::unsafe_variant_cast<C>(&componentVector[componentIndex]);
	}

	template <typename S>
	S& ECS::RegisterSystem(Entity* entity)
	{
		const int64_t typeId = Factory::TypeId<char>::GetId<S>();

		if (typeId >= systemVector.capacity())
			systemVector.resize(typeId * 2);

		assert(!systemVector[typeId].is_valid() && "System is already registred");

		//Run time reflection
		rttr::type t = rttr::type::get<S>();
		systemVector[typeId] = t.create();
		return systemVector[typeId].get_value<S>();
	}

	template <typename S>
	void ECS::DeleteSystem(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<char>::GetId<S>();
		assert(systemVector[typeId].is_valid() && "System is not registred");

		systemVector[typeId].clear();
	}

	template <typename S>
	S& ECS::GetSystem(Entity* entity)
	{
		const uint64_t typeId = Factory::TypeId<char>::GetId<S>();
		assert(systemVector[typeId].is_valid() && "System is not registred");

		return systemVector[typeId].get_value<S>();
	}

	template <typename ... Cs,typename F>
	void ECS::ForEach(F&& func)
	{
		const uint64_t componentMask = ComponentMask<Cs...>();

		for (uint64_t i = 0; i < numAliveEntities; ++i)
		{
			if ((entityVector[i].componentMask & componentMask) == componentMask) //Check if entity has components
				func(entityVector[i].dataIndex, GetComponent<Cs>(&entityVector[i])...);
		}
	}


}
