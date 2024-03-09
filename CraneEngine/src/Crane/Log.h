#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace Crane
{
	class CRANE_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()   { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core logger macros
#define CR_CORE_INFO(...)     ::Crane::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CR_CORE_TRACE(...)    ::Crane::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CR_CORE_WARN(...)     ::Crane::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CR_CORE_ERROR(...)    ::Crane::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CR_CORE_CRITICAL(...) ::Crane::Log::GetCoreLogger()->critical(__VA_ARGS__)

//_____________TRACE NOT WORKING IN CLIENT____________________________

//Client logger macros
#define CR_INFO(...)     ::Crane::Log::GetClientLogger()->info(__VA_ARGS__)
#define CR_TRACE(...)    ::Crane::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CR_WARN(...)     ::Crane::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CR_ERROR(...)    ::Crane::Log::GetClientLogger()->error(__VA_ARGS__)
#define CR_CRITICAL(...) ::Crane::Log::GetClientLogger()->critical(__VA_ARGS__)