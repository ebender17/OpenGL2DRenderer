#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace GLCore {

	class GLCORE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// TODO: strip in distribution versions
// TODO: pass in __LINE__ and __FILE__

// Core log macros
#define GLCORE_LOG_TRACE(...)         ::GLCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GLCORE_LOG_INFO(...)          ::GLCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GLCORE_LOG_WARN(...)          ::GLCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GLCORE_LOG_ERROR(...)         ::GLCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GLCORE_LOG_CRITICAL(...)      ::GLCore::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...)         ::GLCore::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::GLCore::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::GLCore::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::GLCore::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::GLCore::Log::GetClientLogger()->critical(__VA_ARGS__)

