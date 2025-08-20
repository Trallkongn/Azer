#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/Shader.h>
#include <Platform/OpenGL/Material/OpenGLMaterial.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Buffer.h>
#include <glm/glm.hpp>

namespace Azer {

	class PBR
	{
	public:
		PBR(Ref<OpenGLMaterial> material);
		~PBR();

		void Render(const glm::vec3& camPos, const glm::mat4& transform, const glm::mat4& viewProjectionMatrix);
	private:
		Ref<Shader> m_PBR_shader;
		Ref<OpenGLMaterial> m_Material;
		Ref<VertexArray> m_VertexArray;
	};
}