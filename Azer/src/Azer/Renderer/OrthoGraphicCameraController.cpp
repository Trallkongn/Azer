#include "azpch.h"
#include "OrthoGraphicCameraController.h"

#include "Azer/Core/Core.h"
#include "Azer/Core/Input.h"
#include "Azer/Core/KeyCodes.h"

namespace Azer {

	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Rotation(rotation), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthoGraphicCameraController::OnUpdate(TimeStep delta)
	{
		// Camera
		if (Input::IsKeyPressed(AZ_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * delta;
		else if (Input::IsKeyPressed(AZ_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * delta;

		if (Input::IsKeyPressed(AZ_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * delta;
		else if (Input::IsKeyPressed(AZ_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * delta;
		
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(AZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * delta;
			else if (Input::IsKeyPressed(AZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * delta;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraSpeed = m_ZoomLevel;
	}

	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthoGraphicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthoGraphicCameraController::OnWindowResized));
	}

	bool OrthoGraphicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthoGraphicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}