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
		:Layer("Example"), m_CameraController(1.6f/0.9f, true)
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

		auto textureShader = m_ShaderLib.Load("assets/shaders/Texture.glsl");

		m_Texture = Azer::Texture2D::Create("assets/textures/feibi2.jpg");
		m_FeiBiTexture = Azer::Texture2D::Create("assets/textures/feibi.jpg");
		m_PlayerTexture = Azer::Texture2D::Create("assets/textures/player2.png");

		std::dynamic_pointer_cast<Azer::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Azer::OpenGLShader>(textureShader)->SetUniformInt(0, "u_Texture");
	}

	void OnUpdate(Azer::TimeStep delta) override
	{
		// Update
		m_CameraController.OnUpdate(delta);
		
		// Render
		Azer::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		Azer::RenderCommand::Clear();

		Azer::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto textureShader = m_ShaderLib.Get("Texture");

		m_Texture->Bind();
		Azer::Renderer::Submit(SquareVA, textureShader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));
		/*m_PlayerTexture->Bind();
		Azer::Renderer::Submit(SquareVA, textureShader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));*/
		m_FeiBiTexture->Bind();
		Azer::Renderer::Submit(SquareVA, textureShader, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f)));

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
		m_CameraController.OnEvent(event);
	}

private:
	Azer::ShaderLibrary m_ShaderLib;
	// Square
	Azer::Ref<Azer::VertexArray> SquareVA;
	Azer::Ref<Azer::VertexBuffer> SquareVB;
	Azer::Ref<Azer::IndexBuffer> SquareIB;
	Azer::Ref<Azer::Shader> m_TextureShader;

	Azer::Ref<Azer::Texture2D> m_Texture, m_PlayerTexture, m_FeiBiTexture;

	Azer::OrthoGraphicCameraController m_CameraController;
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