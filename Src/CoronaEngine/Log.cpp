#include "Log.h"

namespace Corona
{




	void Debug::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		spdlog::set_level(spdlog::level::trace);
	}


}

