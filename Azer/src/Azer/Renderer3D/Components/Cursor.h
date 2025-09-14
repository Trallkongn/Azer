#pragma once

#include <Azer/Core/Core.h>
#include <Azer/Renderer/OrthoGraphicCameraController.h>
#include <Azer/Renderer3D/PerspectiveGraphicCamera.h>

#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Shader.h>

#include <glm/glm.hpp>

namespace Azer {

	class Cursor
	{
	public:
		Cursor();
		~Cursor();

		void Draw(PerspectiveGraphicCamera& camera, const glm::vec3& centerPos, const glm::vec2& viewportSize);

	private:
		OrthoGraphicCameraController m_CamController;

		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
	};
}