#include "SandBox2D.h"
#include "ImGui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Azer/Core/Timer.h"

#define PROFILE_SCOPE(name) Azer::Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_ProfileResults.push_back(profileResult); })

SandBox2D::SandBox2D()
	: m_CameraController(1.6/0.9, true)
{

}

void SandBox2D::OnAttach()
{
	m_Texture1 = Azer::Texture2D::Create("assets/textures/feibi.jpg");
	m_Texture2 = Azer::Texture2D::Create("assets/textures/feibi2.jpg");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(Azer::TimeStep delta)
{
	PROFILE_SCOPE("SandBox2D::OnUpdate");

	// Camera
	{
		PROFILE_SCOPE("CameraController::OnUpdate"); 
		m_CameraController.OnUpdate(delta);
	}
	

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Azer::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
		Azer::RenderCommand::Clear();
	}
	
	{
		PROFILE_SCOPE("Renderer Draw");
		Azer::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Azer::Renderer2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(0.8f, 0.8f), m_Color);
		Azer::Renderer2D::DrawQuad(glm::vec2(-2.0f, 0.0f), glm::vec2(0.8f, 0.8f), m_Texture1);
		Azer::Renderer2D::DrawQuad(glm::vec2(0.8f, -0.5f), glm::vec2(0.6f, 1.0f), m_Color2);
		Azer::Renderer2D::DrawQuad(glm::vec2(1.8f, -0.5f), glm::vec2(0.6f, 1.0f), m_Texture2);

		Azer::Renderer2D::EndScene();
	}
	
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square1 Color", glm::value_ptr(m_Color));
	ImGui::ColorEdit4("Square2 Color", glm::value_ptr(m_Color2));

	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, " %.3fms  ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void SandBox2D::OnEvent(Azer::Event& event)
{
	m_CameraController.OnEvent(event);
}
