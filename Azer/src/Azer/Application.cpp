#include "azpch.h"
#include "Application.h"

#include "Azer/Log.h"


#include <GLFW/glfw3.h>


namespace Azer {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClosed));

		AZ_CORE_INFO("{0}",e.ToString());
	}

	bool Application::onWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}