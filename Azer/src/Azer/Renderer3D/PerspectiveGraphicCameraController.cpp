#include "azpch.h"
#include "PerspectiveGraphicCameraController.h"

#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Core/Input.h>
#include <Azer/Core/KeyCodes.h>

namespace Azer {

	PerspectiveGraphicCameraController::PerspectiveGraphicCameraController(float distance, float fov, float aspectRatio, float nearClip, float farClip, bool rotation)
		: 
		m_Distance(distance),
		m_Fov(fov),
		m_AspectRatio(aspectRatio),
		m_NearClip(nearClip),
		m_FarClip(farClip),
		m_Rotation(rotation),
		m_Camera(m_Fov, m_AspectRatio, m_NearClip, m_FarClip)
	{
		// ÇòÃæ×ø±ê ¡ú µÑ¿¨¶û×ø±ê
		float cosPitch = glm::cos(glm::radians(m_Pitch));
		glm::vec3 eye{
			m_Center.x + m_Distance * cosPitch * glm::cos(glm::radians(m_Yaw)),
			m_Center.y + m_Distance * glm::sin(glm::radians(m_Pitch)),
			m_Center.z + m_Distance * cosPitch * glm::sin(glm::radians(m_Yaw))
		};

		//glm::mat4 viewMatrix = glm::lookAt(eye, m_Center, m_WorldUp);

		m_Camera.SetPosition(eye);
		m_Camera.SetCenter(m_Center);
		m_Camera.SetWorldUp(m_WorldUp);
	}

	void PerspectiveGraphicCameraController::OnUpdate(TimeStep delta)
	{
		if (Input::IsKeyPressed(AZ_KEY_UP))
		{
			m_Pitch += m_CameraRotationSpeed * delta;
			AZ_CORE_INFO("Pitch: {}", m_Pitch);
		}

		if (Input::IsKeyPressed(AZ_KEY_DOWN))
		{
			m_Pitch -= m_CameraRotationSpeed * delta;
			AZ_CORE_INFO("Pitch: {}", m_Pitch);
		}

		if (Input::IsKeyPressed(AZ_KEY_LEFT))
		{
			m_Yaw -= m_CameraRotationSpeed * delta;
			AZ_CORE_INFO("Yaw: {}", m_Yaw);
		}

		if (Input::IsKeyPressed(AZ_KEY_RIGHT))
		{
			m_Yaw += m_CameraRotationSpeed * delta;
			AZ_CORE_INFO("Yaw: {}", m_Yaw);
		}




		// ÇòÃæ×ø±ê ¡ú µÑ¿¨¶û×ø±ê
		float cosPitch = glm::cos(glm::radians(m_Pitch));
		glm::vec3 eye{
			m_Center.x + m_Distance * cosPitch * glm::cos(glm::radians(m_Yaw)),
			m_Center.y + m_Distance * glm::sin(glm::radians(m_Pitch)),
			m_Center.z + m_Distance * cosPitch * glm::sin(glm::radians(m_Yaw))
		};

		//glm::mat4 viewMatrix = glm::lookAt(eye, m_Center, m_WorldUp);

		m_Camera.SetPosition(eye);
		m_Camera.SetCenter(m_Center);
		m_Camera.SetWorldUp(m_WorldUp);	
	}

	void PerspectiveGraphicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveGraphicCameraController::OnMouseScrolled));
	}

	bool PerspectiveGraphicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Distance -= e.GetYOffset() * 3;
		m_Distance = std::min(m_FarClip, m_Distance);
		m_Distance = std::max(m_NearClip, m_Distance);

		AZ_CORE_INFO("{}", e.ToString());
		AZ_CORE_INFO("distance : {}", m_Distance);

		return false;
	}

	bool PerspectiveGraphicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

}