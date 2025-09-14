#include "azpch.h"
#include "PerspectiveGraphicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Azer {
	PerspectiveGraphicCamera::PerspectiveGraphicCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveGraphicCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip) 
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveGraphicCamera::ReCalculateViewMatrix() {
		m_ViewMatrix = glm::lookAt(m_Position, m_Center, m_WorldUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

} // namespace Azer