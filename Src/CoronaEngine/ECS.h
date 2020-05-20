#pragma once
#include <cstdint>
#include <array>
#include <any>
#include <unordered_map>
#include <functional>
#include <rttr/registration>
#include "IdFactory.h"
#include <limits>
constexpr auto MAX_ENTITIES = 1024;
constexpr auto MAX_COMPONENTS_PER_ENTITY = 64;
constexpr auto MAX_SYSTEMS = 64;
constexpr auto DEDICATED_MEMORY_FOR_COMPONENTS = 8 * 1024 * 1024 * 1024;



namespace Corona
{

	//std::apply([](auto ...x){std::make_tuple(x.do_something()...);} , the_tuple); Iterate over tuple


	class ECS
	{
	public:
		struct Entity
		{
			uint64_t  id = std::numeric_limits<uint64_t>::max();
			uint64_t  dataIndex = std::numeric_limits<uint64_t>::max();
			uint64_t  index = std::numeric_limits<uint64_t>::max();
			uint64_t componentMask = 0;

			uint16_t  isAlive = 0;
			uint16_t  pendingDeath = 0;  //If true, the entity is to be terminated
		};
	private:
		static std::array<Entity, MAX_ENTITIES>				entityPool;
		static std::array<rttr::variant, MAX_SYSTEMS>		systemPool;
		static std::unordered_map<uint64_t, rttr::variant*>	componentMap;


		static int64_t numAliveEntities;


	public:
		template< typename... Cs >
		static constexpr uint64_t ComponentMask();
		static void Tick();
		static void Refresh();

		static int64_t GetNumAliveEntities();

		static Entity*  RegisterEntity();
		static void		DeleteEntity(Entity* entity);

		/**
		 * \brief Registers and attaches a component upon an entity
		 * \param[in] entity The entity to attach component to
		 * \tparam C The type of the component
		 * \return A reference to the new component
		 */
		template <typename C>
		static C&   RegisterComponent(Entity* entity);
		template <typename C>
		static void DeleteComponent(Entity* entity);
		template <typename C>
		static C&	GetComponent(Entity* entity);

		template <typename S>
		static S& RegisterSystem(Entity* entity);
		template <typename S>
		static void DeleteSystem(Entity* entity);
		template <typename S>
		static S& GetSystem(Entity* entity);

		template <typename... Cs>
		static void ForEach(std::function<void(Cs &...)> function);
	};


}

#include "ECS.inl"