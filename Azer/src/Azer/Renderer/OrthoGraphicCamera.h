#pragma once

#include "GraphicCamera.h"
#include "glm/glm.hpp"

namespace Azer {

	class OrthoGraphicCamera : public GraphicCamera
	{
	public:
		OrthoGraphicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) override { m_Position = position; ReCalculateViewMatrix(); }
		const glm::vec3& GetPosition() const override { return m_Position; }

		void SetRotation(float rotation) { m_Rotation = rotation; ReCalculateViewMatrix(); }
		float GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }
	private:
		void ReCalculateViewMatrix() override;
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		float m_Rotation = 0.0f;
	};
}