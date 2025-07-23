#include "azpch.h"
#include "WindowsWindow.h"

#include "Azer/Events/ApplicationEvent.h"
#include "Azer/Events/KeyEvent.h"
#include "Azer/Events/MouseEvent.h"
#include "../OpenGL/OpenGLContext.h"

#include "Azer/Renderer/Renderer.h"

// 目标宽高比 (例如16:9)
const float ASPECT_RATIO = 16.0f / 9.0f;
// 最小窗口尺寸
const uint32_t MIN_WIDTH = 900;
const uint32_t MIN_HEIGHT = static_cast<uint32_t>(MIN_WIDTH / ASPECT_RATIO);
static float WindowArea;


namespace Azer {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		AZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();

	}

	void WindowsWindow::SetVSync(bool enable)
	{
		if (enable) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_Data.VSync = enable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		AZ_CORE_INFO("Created window {0} ({1},{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			AZ_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		
		WindowArea = props.Width * props.Height;

		// Azer make Windows Context API
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			float newRatio = static_cast<float>(width) / height;
			float newArea = width * height;

			if (newRatio > ASPECT_RATIO && newArea > WindowArea)
				height = width / ASPECT_RATIO;
			else if(newRatio < ASPECT_RATIO && newArea > WindowArea)
				width = height * ASPECT_RATIO;
			else if(newRatio < ASPECT_RATIO && newArea < WindowArea)
				height = height = width / ASPECT_RATIO;
			else if(newRatio > ASPECT_RATIO && newArea < WindowArea)
				width = height * ASPECT_RATIO;

			WindowArea = newArea;
			// 确保窗口不小于最小尺寸
			width = std::max((uint32_t)width, MIN_WIDTH);
			height = std::max((uint32_t)height, MIN_HEIGHT);

			// 调整视口以匹配新的窗口尺寸
			RenderCommand::SetViewport(0, 0, width, height);

			// 如果实际尺寸与请求的尺寸不同，则调整窗口
			int current_width, current_height;
			glfwGetWindowSize(window, &current_width, &current_height);
			if (current_width != width || current_height != height)
			{
				// 临时关闭回调以避免循环
				glfwSetWindowSizeCallback(window, nullptr);
				glfwSetWindowSize(window, width, height);
				glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
					{
						WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
						data.Width = width;
						data.Height = height;

						WindowResizeEvent event(width, height);
						data.EventCallback(event);
					});
			}

			// 输出当前窗口尺寸和比例
			AZ_CORE_TRACE("Window resized to : {0} x {1} (aspect ratio: {2})", width, height, static_cast<float>(width) / height);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{

	}
}
