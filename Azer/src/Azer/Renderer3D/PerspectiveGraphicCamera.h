#pragma once

#include "Azer/Core/Core.h"
#include "glm/glm.hpp"

#include <Azer/Renderer/GraphicCamera.h>

namespace Azer {
	class PerspectiveGraphicCamera : public GraphicCamera
	{
	public:
		PerspectiveGraphicCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);

		inline void SetPosition(const glm::vec3& position) override { m_Position = position; ReCalculateViewMatrix(); }
		inline const glm::vec3& GetPosition() const override { return m_Position; }
		
		inline void SetCenter(const glm::vec3& center) { m_Center = center; ReCalculateViewMatrix(); }
		inline const glm::vec3& GetCenter() const { return m_Center; }
		
		inline void SetWorldUp(const glm::vec3& worldUp) { m_WorldUp = worldUp; ReCalculateViewMatrix(); }
		inline const glm::vec3& GetWorldUp() const { return m_WorldUp; }
		
		inline const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	private:
		void ReCalculateViewMatrix() override;

	private:
		glm::mat4 m_ProjectionMatrix; // Projection matrix
		glm::mat4 m_ViewMatrix;       // View matrix
		glm::mat4 m_ViewProjectionMatrix; // Combined view-projection matrix

		glm::vec3 m_Position = { 50,50,50 }; // Camera position in world space
		glm::vec3 m_Center = { 0, 0, 0 };
		glm::vec3 m_WorldUp = { 0, 1, 0 };
	};
}