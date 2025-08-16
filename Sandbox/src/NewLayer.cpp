#include "NewLayer.h"
#include <imgui/imgui.h>
#include "glm/gtc/type_ptr.hpp"

NewLayer::NewLayer(const std::string& name)
	: m_CameraController(50,glm::radians(45.0f),1920.0/1080.0,0.1f,100.0f,false)
{

}

NewLayer::~NewLayer()
{
}

void NewLayer::OnAttach()
{
}

void NewLayer::OnDetach()
{
}

void NewLayer::OnUpdate(TimeStep delta)
{
	m_CameraController.SetCameraRotationSpeed(m_CameraRotationSpeed);
	m_CameraController.OnUpdate(delta);

	Renderer3D::BeginScene(m_CameraController.GetCamera());

	Renderer3D::DrawCubeWithEdge(glm::vec3(0,0,0), glm::vec3(m_Scale), m_Color, m_EdgeColor);

	Renderer3D::EndScene();
}

void NewLayer::OnRender()
{
}

void NewLayer::OnImGuiRender()
{
	ImGui::Begin("Test");

	ImGui::ColorEdit4("color", glm::value_ptr(m_Color));
	ImGui::ColorEdit4("edge_color", glm::value_ptr(m_EdgeColor));
	ImGui::SliderFloat("scale", &m_Scale,0.1f,1.5f);
	ImGui::SliderFloat("Camera Rotation Speed", &m_CameraRotationSpeed, 0.0f, 100.0f);

	ImGui::End();
}

void NewLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}
