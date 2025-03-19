#include "azpch.h"
#include "Application.h"
#include "Azer/Log.h"
#include "Azer/Events/ApplicationEvent.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // ADD THIS LINE	

namespace Azer {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		// 由于未知原因，使用trace之前必须加上set_level，否则不会打印任何内容
		spdlog::set_level(spdlog::level::trace);
		AZ_TRACE(e.ToString());
		while (true);
	}

}