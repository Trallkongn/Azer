#include "NewLayer.h"
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"

NewLayer::NewLayer(const std::string& name)
	: Layer(name), m_CameraController(50, 45.0f, 1920.0 / 1080.0, 0.1f, 1000.0f, false)
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
	m_CameraController.SetCenter(m_Center);
	m_CameraController.SetCameraRotationSpeed(m_CameraRotationSpeed);
	m_CameraController.OnUpdate(delta);
    
	Renderer3D::BeginScene(m_CameraController.GetCamera());

	Renderer3D::DrawCursor(m_CameraController.GetCamera(), m_Center);

	if (m_EnableGrid)
	{
		Renderer3D::DrawWorldGrid(m_CameraController.GetCamera());
	}

	if (m_EnvStatu)
	{
		Renderer3D::DrawEnv();
	}

	if (m_PBRstatu)
	{
		Renderer3D::DrawPBR(m_Position, glm::vec3(m_Scale), m_Rotation, m_Metalness, m_AO, m_Roughness, m_Exposure);
	}
	else if (m_ToonStatu)
	{
		Renderer3D::DrawToon(m_Position, glm::vec3(m_Scale), m_Rotation);
	}
	else
	{
		Renderer3D::DrawCube(m_Position, glm::vec3(m_Scale), m_Color, m_Rotation);
	}
	
	Renderer3D::EndScene();
}

void NewLayer::OnRender()
{

}

void NewLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::Begin("Global Settings");

	ImGui::Checkbox("Enable Env", &m_EnvStatu);
	ImGui::Checkbox("Enable Grid", &m_EnableGrid);
	ImGui::InputFloat3("Center", glm::value_ptr(m_Center));

	ImGui::End();

	ImGui::Begin("Object Settings");

	ImGui::Checkbox("Enable PBR", &m_PBRstatu);
	ImGui::Checkbox("Enable Toon", &m_ToonStatu);
	ImGui::ColorEdit4("Object Color", glm::value_ptr(m_Color));
	ImGui::DragFloat3("Object Position", glm::value_ptr(m_Position), 0.05f, -10, 10);
	ImGui::DragFloat3("Object Rotation", glm::value_ptr(m_Rotation), 1.0f, -180, 180);
	ImGui::SliderFloat("Object Scale", &m_Scale, 0.1f, 5.0f);
	ImGui::SliderFloat("Camera Rotation Speed", &m_CameraRotationSpeed, 0.0f, 100.0f);

	ImGui::End();

	ImGui::Begin("PBR");

	ImGui::SliderFloat("Metalness", &m_Metalness, 0.0f, 1.0f);
	ImGui::SliderFloat("Roughness", &m_Roughness, 0.0f, 1.0f);
	ImGui::SliderFloat("AO", &m_AO, 0.0f, 1.0f);
	ImGui::SliderFloat("Exposure", &m_Exposure, 1.0f, 10.0f);

	ImGui::End();
	ImGui::End();
}

void NewLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}
