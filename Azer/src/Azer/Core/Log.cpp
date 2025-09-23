#include "azpch.h"

#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Azer {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<ImGuiLogSink> Log::s_ImGuiSink;
	

	void Log::Init()
	{
		/*spdlog::set_pattern("%^[%T] %n: %v%$");*/

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_pattern("%^[%T] %n: %v%$");
		s_ImGuiSink = std::make_shared<ImGuiLogSink>();
		s_ImGuiSink->set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = std::make_shared<spdlog::logger>(
			"Azer", spdlog::sinks_init_list{ console_sink, s_ImGuiSink });
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>(
			"APP", spdlog::sinks_init_list{ console_sink, s_ImGuiSink });
		s_ClientLogger->set_level(spdlog::level::trace);

		spdlog::register_logger(s_CoreLogger);
		spdlog::register_logger(s_ClientLogger);
	}
}