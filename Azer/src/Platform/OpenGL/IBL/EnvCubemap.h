#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Azer {

	class EnvCubeMap
	{
	public:
		EnvCubeMap(uint32_t width, uint32_t height, const std::string& HDRpath, Ref<VertexArray> vertexArray);
		~EnvCubeMap();

		void BakeRender();
		void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

		inline Ref<CubeMap> GetCubeMap() const { return m_CubeMap; }
		inline uint32_t GetRendererID() const { return m_CubeMap->GetRendererID(); }
	private:

		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_HDRpath;

		uint32_t m_RendererID;
		uint32_t m_RendererID2;

		Ref<Shader> m_HDR_env_shader;
		Ref<Shader> m_SkyBoxShader;
		Ref<Texture2D> m_HDR_texture;
		Ref<VertexArray> m_VertexArray;
		Ref<CubeMap> m_CubeMap;
		Ref<FrameBufferRenderBuffer> m_fbrb;

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
