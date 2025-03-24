#include "azpch.h"
#include "Application.h"

#include "Azer/Log.h"
#include "Azer/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>


namespace Azer {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	// 由于未知原因，使用trace之前必须加上set_level，否则不会打印任何内容
	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}