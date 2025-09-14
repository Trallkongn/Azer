#pragma once

#include "Azer/Core/Core.h"
#include "PerspectiveGraphicCamera.h"
#include <Azer/Core/TimeStep.h>
#include <Azer/Events/Event.h>
#include <Azer/Events/MouseEvent.h>
#include <Azer/Events/ApplicationEvent.h>
#include <glm/glm.hpp>

namespace Azer {
	
	class PerspectiveGraphicCameraController
	{
	public:
		PerspectiveGraphicCameraController(float distance, float fov, float aspectRatio, float nearClip, float farClip, bool rotation);

		void OnUpdate(TimeStep delta);
		void OnEvent(Event& e);

		inline PerspectiveGraphicCamera& GetCamera() { return m_Camera; }
		inline const PerspectiveGraphicCamera& GetCamera() const { return m_Camera; }

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }
		
		inline void SetPitch(float pitch) { m_Pitch = pitch; }
		inline float GetPitch() const { return m_Pitch; }
		
		inline void SetYaw(float yaw) { m_Yaw = yaw; }
		inline float GetYaw() const { return m_Yaw; }
		
		inline void SetCenter(const glm::vec3& center) { m_Center = center; }

		inline void SetCameraRotationSpeed(float speed) { m_CameraRotationSpeed = speed; }
		inline float GetCameraRotationSpeed() const { return m_CameraRotationSpeed; }

		inline float GetAspectRatio() const { return m_AspectRatio; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

	private:

		float m_Fov;
		float m_AspectRatio;
		float m_NearClip;
		float m_FarClip;

		// 维护 center / distance / pitch / yaw

		glm::vec3  m_Center = { 0,0,0 };        // 始终盯着的世界坐标系点
		float      m_Distance;      // 相机到 center 的距离（>0）
		float      m_Pitch = 0.0f;         // 俯仰角（-89°~89°）
		float      m_Yaw = 0.0f;           // 水平角（0~360°）
		glm::vec3  m_WorldUp = { 0,1,0 };       // 通常 {0,1,0}

		PerspectiveGraphicCamera m_Camera;

		bool m_Rotation = false;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraSpeed = 1.0f;
		float m_CameraRotationSpeed = 50.0f;

		float last_xpos = 0.0;
		float last_ypos = 0.0;
	};
}