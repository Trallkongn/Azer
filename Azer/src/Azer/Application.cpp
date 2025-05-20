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

	// ����δ֪ԭ��ʹ��trace֮ǰ�������set_level�����򲻻��ӡ�κ�����
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