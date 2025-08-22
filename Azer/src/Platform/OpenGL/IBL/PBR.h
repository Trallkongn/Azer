#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Material/Material.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Buffer.h>
#include <glm/glm.hpp>

namespace Azer {

	class PBR
	{
	public:
		PBR();
		~PBR();

		void Render(uint32_t iMap, uint32_t pMap, uint32_t brdfLUT, const glm::vec3& camPos, const glm::mat4& transform, const glm::mat4& viewProjectionMatrix);
	private:
		Ref<Shader> m_PBR_shader;
		Material m_Material;
		Ref<VertexArray> m_VertexArray;
	};
}