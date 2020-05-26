#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
namespace Corona
{


	
	class Debug
	{
	public:
		static void Init();
	};
}
#define DEBUG_TRACE(...)	::spdlog::trace(__VA_ARGS__)
#define DEBUG_INFO(...)		::spdlog::info(__VA_ARGS__)
#define DEBUG_WARN(...)		::spdlog::warn(__VA_ARGS__)
#define DEBUG_ERROR(...)	::spdlog::error(__VA_ARGS__)
#define DEBUG_FATAL(...)	::spdlog::fatal(__VA_ARGS__)

#ifdef NDEBUG
#define DEBUG_TRACE(...)
#define DEBUG_INFO(...)	
#define DEBUG_WARN(...)	
#define DEBUG_ERROR(...)
#define DEBUG_FATAL(...)
#endif