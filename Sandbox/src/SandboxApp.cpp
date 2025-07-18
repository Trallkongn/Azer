#include <Azer.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <memory>

class ExampleLayer : public Azer::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// Square
		SquareVA.reset(Azer::VertexArray::Create());

		float SquareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //0
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //1
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, //2
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  //3
		};

		SquareVB.reset(Azer::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));

		Azer::BufferLayout SquareLayout = {
			{Azer::ShaderDataType::Float3, "a_Position"},
			{Azer::ShaderDataType::Float2, "a_TexCoord"}
		};

		SquareVB->SetLayout(SquareLayout);
		SquareVA->AddVertexBuffer(SquareVB);

		uint32_t SquareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};
		SquareIB.reset(Azer::IndexBuffer::Create(SquareIndices, 6));
		SquareVA->SetIndexBuffer(SquareIB);

		m_TextureShader.reset(Azer::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Azer::Texture2D::Create("assets/textures/ike.jpg");
		m_FeiBiTexture = Azer::Texture2D::Create("assets/textures/feibi.jpg");
		m_PlayerTexture = Azer::Texture2D::Create("assets/textures/player2.png");

		std::dynamic_pointer_cast<Azer::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Azer::OpenGLShader>(m_TextureShader)->SetUniformInt(0, "u_Texture");
	}

	void OnUpdate(Azer::TimeStep delta) override
	{
		// Camera
		if (Azer::Input::IsKeyPressed(AZ_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraSpeed * delta;
			//m_CameraRotation -= m_CameraRotationSpeed * delta;
		}
		else if (Azer::Input::IsKeyPressed(AZ_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraSpeed * delta;
			//m_CameraRotation += m_CameraRotationSpeed * delta;
		}
		else if (Azer::Input::IsKeyPressed(AZ_KEY_UP))
		{
			m_CameraPosition.y += m_CameraSpeed * delta;
		}
		else if (Azer::Input::IsKeyPressed(AZ_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraSpeed * delta;
		}
		
		Azer::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		Azer::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Azer::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_Texture->Bind();
		Azer::Renderer::Submit(SquareVA, m_TextureShader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.3f)));
		m_PlayerTexture->Bind();
		Azer::Renderer::Submit(SquareVA, m_TextureShader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.3f)));

		m_FeiBiTexture->Bind();
		Azer::Renderer::Submit(SquareVA, m_TextureShader, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f))*glm::scale(glm::mat4(1.0f), glm::vec3(1.3f)));

		//Azer::Renderer::Submit(m_VertexArray, m_Shader);
		Azer::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
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
	// Square
	Azer::Ref<Azer::VertexArray> SquareVA;
	Azer::Ref<Azer::VertexBuffer> SquareVB;
	Azer::Ref<Azer::IndexBuffer> SquareIB;
	Azer::Ref<Azer::Shader> m_TextureShader;

	Azer::Ref<Azer::Texture2D> m_Texture, m_PlayerTexture, m_FeiBiTexture;

	Azer::OrthoGraphicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 2.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 200.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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