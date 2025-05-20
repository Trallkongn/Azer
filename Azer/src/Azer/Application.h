#pragma once

#include "azpch.h"

#include "Core.h"
#include "Azer/Events/Event.h"
#include "Window.h"
#include "Azer/Events/ApplicationEvent.h"

namespace Azer {

	class AZER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool onWindowClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
