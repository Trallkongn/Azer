#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Renderer/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Azer {

	class IrradianceMap
	{
	public:
		IrradianceMap(uint32_t width, uint32_t height, Ref<CubeMap> envCubemap, Ref<VertexArray> vertexArray);
		~IrradianceMap();

		void BakeRender();
		void Render();

		inline Ref<CubeMap> GetCubeMap() const { return m_CubeMap; }
		inline uint32_t GetRendererID() const { return m_CubeMap->GetRendererID(); }

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		Ref<CubeMap> m_CubeMap;
		Ref<FrameBufferRenderBuffer> m_fbrb;
		Ref<Shader> m_IrradianceMapShader;
		Ref<CubeMap> m_EnvCubemap;

		Ref<VertexArray> m_VertexArray;

		float* m_Vertices;
		uint32_t m_Count;

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