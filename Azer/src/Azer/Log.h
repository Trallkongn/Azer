#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Azer {

	class AZER_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};

}

// Core log macros
#define AZ_CORE_ERROR(...)	 ::Azer::Log::GetCoreLogger()->error(__VA_ARGS__)
#define AZ_CORE_INFO(...)	 ::Azer::Log::GetCoreLogger()->info(__VA_ARGS__)
#define AZ_CORE_TRACE(...)	 ::Azer::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define AZ_CORE_WARN(...)	 ::Azer::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define AZ_CORE_FATAL(...)	 ::Azer::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define AZ_ERROR(...)	 ::Azer::Log::GetClientLogger()->error(__VA_ARGS__)
#define AZ_INFO(...)	 ::Azer::Log::GetClientLogger()->info(__VA_ARGS__)
#define AZ_TRACE(...)	 ::Azer::Log::GetClientLogger()->trace(__VA_ARGS__)
#define AZ_WARN(...)	 ::Azer::Log::GetClientLogger()->warn(__VA_ARGS__)
#define AZ_FATAL(...)	 ::Azer::Log::GetClientLogger()->critical(__VA_ARGS__)