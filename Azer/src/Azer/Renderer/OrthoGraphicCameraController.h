#pragma once

#include "Azer/Renderer/OrthoGraphicCamera.h"
#include "Azer/Core/TimeStep.h"

#include "Azer/Events/ApplicationEvent.h"
#include "Azer/Events/MouseEvent.h"

#include "glm/glm.hpp"

namespace Azer {

	class OrthoGraphicCameraController
	{
	public:
		OrthoGraphicCameraController(float aspectRatio, bool rotation = false); 

		void OnUpdate(TimeStep delta);
		void OnEvent(Event& e);

		inline OrthoGraphicCamera& GetCamera() { return m_Camera; }
		inline const OrthoGraphicCamera& GetCamera() const { return m_Camera; }

		inline void SetZoomLevel(float level) { m_ZoomLevel = level; }
		inline float GetZoomLevel() const { return m_ZoomLevel; }

		inline float GetAspectRatio() const { return m_AspectRatio; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthoGraphicCamera m_Camera;

		bool m_Rotation = false;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;
	};
}