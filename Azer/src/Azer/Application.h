#pragma once

#include "azpch.h"

#include "Core.h"
#include "Azer/Events/Event.h"
#include "Window.h"

namespace Azer {

	class AZER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
