#pragma once

#include <Azer/Core/Core.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/VertexArray.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Azer {

	class PrefilterMap
	{
	public:
		PrefilterMap(uint32_t width, uint32_t height, int miplevels, Ref<CubeMap> environmentMap, Ref<VertexArray> vertexArray);
		~PrefilterMap();

		void BakeRender();
		void Render();

		inline Ref<CubeMap> GetCubeMap() const { return m_CubeMap; }
		inline uint32_t GetRendererID() const { return m_CubeMap->GetRendererID(); }

	private:

		int m_Miplevels;

		uint32_t m_Width;
		uint32_t m_Height;

		Ref<CubeMap> m_CubeMap;
		Ref<Shader> m_PrefilterMapShader;
		Ref<FrameBufferRenderBuffer> m_fbrb;
		Ref<CubeMap> m_EnvironmentMap;
		Ref<VertexArray> m_VertexArray;

	private:
		glm::mat4 m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 m_CaptureViews[6] = {
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +X
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // -X
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)), // -Y
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +Z
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f))  // -Z
		};
	};
}