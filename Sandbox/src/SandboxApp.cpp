#include <Azer.h>

#include "imgui/imgui.h"
#include <memory>

class ExampleLayer : public Azer::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// Vertex Array
		m_VertexArray.reset(Azer::VertexArray::Create());

		// Vertex Buffer
		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.5f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.2f, 0.1f, 0.5f, 1.0f,
		};

		m_VertexBuffer.reset(Azer::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();

		// BufferLayout
		Azer::BufferLayout layout = {
			{Azer::ShaderDataType::Float3, "a_Position"},
			{Azer::ShaderDataType::Float4, "a_Color"}
		};
		m_VertexBuffer->SetLayout(layout);

		// Add VertexBuffer
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		unsigned int indices[3] = {
			0,1,2
		};
		m_IndexBuffer.reset(Azer::IndexBuffer::Create(indices, 3));

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

		m_Shader = std::make_unique<Azer::Shader>(vertexSrc, fragmentSrc);
		m_Shader->Bind();
	}

	void OnUpdate(Azer::TimeStep delta) override
	{
		if (Azer::Input::IsKeyPressed(AZ_KEY_LEFT))
		{
			m_CameraRotation -= m_RotationSpeed * delta;
		}
		else if (Azer::Input::IsKeyPressed(AZ_KEY_RIGHT))
		{
			m_CameraRotation += m_RotationSpeed * delta;
		}
		else if (Azer::Input::IsKeyPressed(AZ_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraSpeed * delta;
		}
		else if (Azer::Input::IsKeyPressed(AZ_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraSpeed * delta;
		}
		
		Azer::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		Azer::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Azer::Renderer::BeginScene(m_Camera);
		Azer::Renderer::Submit(m_VertexArray, m_Shader);
		Azer::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		
	}

	void OnEvent(Azer::Event& event) override
	{
		Azer::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Azer::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Azer::KeyPressedEvent event)
	{
		return false;
	}

private:
	std::shared_ptr<Azer::VertexArray> m_VertexArray;
	std::shared_ptr<Azer::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Azer::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Azer::Shader> m_Shader;

	Azer::OrthoGraphicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraSpeed = 2.0f;
	float m_RotationSpeed = 200.0f;
};

class Sandbox : public Azer::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Azer::Application* Azer::CreateApplication()
{
	return new Sandbox;
}