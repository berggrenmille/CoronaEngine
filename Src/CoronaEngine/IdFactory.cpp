#include "IdFactory.h"
namespace Corona::Factory
{
	std::list<uint64_t> DataIndex::freeIndices = {};
	uint64_t DataIndex::counter = 0;
}