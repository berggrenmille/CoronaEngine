#pragma once
#include <cstdint>
#include <list>

#include <rttr/registration.h>


namespace Corona::Factory
{
	/// <summary>
	/// An utility class to easily generate linear Id's for a certain classes.
	/// The Factory has the ability to create Id's within sub-groups. For example Components and Entities can have their Id generation separate from each other.
	/// The Id's are generated in a linear order: 0, 1, 2, 3... And Id's can be reused.
	/// </summary>
	template <typename T> class Id
	{
	public:
		static uint64_t GetId()
		{
			uint64_t id;
			if (freeIds.empty())
				id = counter++;
			else //Reuse an old id
			{
				id = freeIds.back();
				freeIds.pop_back();
			}
			return id;
		}
		static void FreeId(uint64_t id)
		{
			freeIds.push_back(id);
		}
	private:
		static std::list<uint64_t> freeIds;
		static uint64_t counter;
	};

	/// <summary>
	/// Similar to IdFactory, but instead serves a purpose to both generate and store unique id's for a class.
	/// TypeIdFactory can generate persistent ID's for primarily classes having the same parent.
	/// </summary>
	template <typename T> class TypeId
	{
	public:
		template<typename U>
		static uint64_t GetId()
		{
			static const uint64_t id = counter++;
			return id;
		};
		template<typename U>
		static uint64_t GetFlag()
		{
			static const uint64_t id = TypeId<T>::template GetId<U>();
			static const uint64_t flag = exp2(id); //Return id in powers of 2 so that it can represent a flag
			return flag;
		};
	private:
		static uint64_t counter;
	};


	class DataIndex
	{
	public:
		static uint64_t GetIndex()
		{
			uint64_t index;
			if (freeIndices.empty())
				index = counter++;
			else //Reuse an old index
			{
				index = freeIndices.back();
				freeIndices.pop_back();
			}
			return index;
		}
		static void FreeIndex(uint64_t index)
		{
			freeIndices.push_back(index);
		}
	private:
		static std::list<uint64_t> freeIndices;
		static uint64_t counter;
	};


	template<typename T>
	std::list<uint64_t> Id<T>::freeIds = {};
	template<typename T>
	uint64_t TypeId<T>::counter = 0;
	template<typename T>
	uint64_t Id<T>::counter = 0;



}
