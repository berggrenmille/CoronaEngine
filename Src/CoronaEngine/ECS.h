#pragma once
#include <cstdint>
#include <array>
#include <any>
#include <unordered_map>
#include <functional>
#include <rttr/registration>
#include "IdFactory.h"
#include <limits>
#include <unordered_set>
#include <queue>

constexpr auto MAX_COMPONENTS_PER_ENTITY = 64;

constexpr auto DEDICATED_MEMORY_FOR_COMPONENTS = 8 * 1024 * 1024 * 1024;



namespace Corona
{

	//std::apply([](auto ...x){std::make_tuple(x.do_something()...);} , the_tuple); Iterate over tuple


	class ECS
	{
	public:
		struct Entity
		{
			int64_t dataIndex = -1;
			int64_t componentMask = 0;
			bool alive			=  false;
		};


	private:		
		int64_t reservedEntities = 64;
		int64_t numAliveEntities = 0;
		std::vector<Entity>				entityVector				= std::vector<Entity>(reservedEntities);
		std::vector<int64_t>			entityDataIndexToIndex		= std::vector<int64_t>(reservedEntities);
		std::vector<rttr::variant>		componentVector				= std::vector<rttr::variant>(reservedEntities * MAX_COMPONENTS_PER_ENTITY);
		std::vector<rttr::variant>		systemVector;

		uint64_t	GetDataIndex();
		void		FreeDataIndex(uint64_t index);
		uint64_t	ReserveDataIndex(uint64_t index);
		std::unordered_set<uint64_t> reservedDataIndices;
		std::queue<uint64_t>		 freeDataIndices;
		int64_t						 dataIndexCounter = 0;

	public:
		template< typename... Cs >
		constexpr uint64_t ComponentMask();
		
		void		Refresh();

		[[nodiscard]] int64_t		GetNumAliveEntities() const;

		Entity*		RegisterEntity();
		void		DeleteEntity(Entity* entity);

		/**
		 * \brief Registers and attaches a component upon an entity
		 * \param[in] entity The entity to attach component to
		 * \tparam C The type of the component
		 * \return A reference to the new component
		 */
		template <typename C>
		C&   RegisterComponent(Entity* entity);
		template <typename C>
		void DeleteComponent(Entity* entity);
		template <typename C>
		C&	GetComponent(Entity* entity);

		template <typename S>
		S& RegisterSystem(Entity* entity);
		template <typename S>
		void DeleteSystem(Entity* entity);
		template <typename S>
		S& GetSystem(Entity* entity);

		template <typename ... Cs, typename F>
		void ForEach(F&& func);
	};


}

#include "ECS.inl"