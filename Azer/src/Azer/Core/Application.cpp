#include "azpch.h"
#include "Application.h"
#include "Azer/Core/Log.h"

#include "Azer/Core/Input.h"
#include "Azer/Core/KeyCodes.h"
#include "Azer/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Azer {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		AZ_PROFILE_FUNCTION();

		AZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create thread pool
		m_ThreadPool = std::make_unique<ThreadPool>(2);

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		AZ_PROFILE_FUNCTION();

		Renderer::Shutdown();
		m_ThreadPool->shutdown();
	}

	void Application::Run()
	{
		AZ_PROFILE_FUNCTION();

		while (m_Running)
		{

			AZ_PROFILE_SCOPE("Run loop");
			float time = (float)glfwGetTime();
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			RenderCommand::SetClearColor(m_ClearColor);
			RenderCommand::Clear();


			if (!m_Minimized)
			{
				{
					AZ_PROFILE_SCOPE("Layer OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timeStep);
				}
				

				{
					AZ_PROFILE_SCOPE("ImGui Layer OnUpdate");
					// ImGui Layer
					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack) layer->OnImGuiRender();
					ImGui::Begin("Core Set");
					ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
					ImGui::End();
					m_ImGuiLayer->End();
				}
				
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		AZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));

		//AZ_CORE_TRACE("{0}",e.ToString());

		// from out to inner
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		AZ_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		AZ_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::onWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		AZ_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}