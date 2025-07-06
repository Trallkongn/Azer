#include "azpch.h"
#include "Application.h"
#include "Azer/Log.h"

#include "Azer/Input.h"
#include "Azer/KeyCodes.h"
#include "Renderer/Renderer.h"



namespace Azer {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		AZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		// Vertex Array
		m_VertexArray.reset(VertexArray::Create());

		// Vertex Buffer
		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.5f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.2f, 0.1f, 0.5f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();

		// BufferLayout
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(layout);

		// Add VertexBuffer
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		unsigned int indices[3] = {
			0,1,2
		};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));

		// Set IndexBuffer
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position,1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
		m_Shader->Bind();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
			RenderCommand::Clear();

			/*m_Camera.SetRotation(m_Rotation);
			m_Rotation += 30.0f;
			if (m_Rotation > 360.0f) m_Rotation -= 360.0f;*/

			m_Camera.SetPosition({ x,y,z });

			Renderer::BeginScene(m_Camera);
			Renderer::Submit(m_VertexArray, m_Shader);
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack) layer->OnUpdate();

			// ImGui Layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClosed));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyPressed));

		AZ_CORE_TRACE("{0}",e.ToString());

		// from out to inner
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::onWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		if (Input::IsKeyPressed(AZ_KEY_W))			y += 0.1;
		else if (Input::IsKeyPressed(AZ_KEY_S))		y -= 0.1;
		else if (Input::IsKeyPressed(AZ_KEY_A))		x -= 0.1;
		else if (Input::IsKeyPressed(AZ_KEY_D))		x += 0.1;
		return false;
	}

}