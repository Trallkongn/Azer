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

}