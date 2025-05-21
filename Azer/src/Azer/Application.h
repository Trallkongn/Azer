#pragma once

#include "azpch.h"

#include "Core.h"
#include "Azer/Events/Event.h"
#include "Window.h"
#include "Azer/Events/ApplicationEvent.h"
#include "Azer/LayerStack.h"

namespace Azer {

	class AZER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool onWindowClosed(WindowCloseEvent& e);	
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
