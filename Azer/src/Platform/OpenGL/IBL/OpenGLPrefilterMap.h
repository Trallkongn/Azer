#pragma once

#include <Azer/Core/Core.h>
#include <Azer/Renderer/IBL.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/VertexArray.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Azer {

	class OpenGLPrefilterMap : public PrefilterMap
	{
	public:
		OpenGLPrefilterMap(uint32_t miplevels, uint32_t width, uint32_t height,
			uint32_t skyboxID,
			Ref<VertexArray> vertexArray,
			const glm::mat4& captureProjection,
			const std::vector<glm::mat4>& captureViews
		);
		~OpenGLPrefilterMap();

		void BakeRender() override;

		inline uint32_t GetRendererID() const override { return m_RendererID; }
		inline uint32_t GetMiplevels() const override { return m_Miplevels; }
		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }
		inline void SetCaptureProjection(const glm::mat4& captureProjection) override { m_CaptureProjection = captureProjection; }
		inline void SetCaptureViews(const std::vector<glm::mat4>& captureViews) override { m_CaptureViews = captureViews; }
		inline Ref<VertexArray> GetVertexArray() const override { return m_VertexArray; }

	private:

		int m_Miplevels;

		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t in_skyboxID;
		uint32_t m_RendererID;

		Ref<CubeMap> m_CubeMap;
		Ref<Shader> m_PrefilterMapShader;
		Ref<FrameBufferRenderBuffer> m_fbrb;
		Ref<VertexArray> m_VertexArray;

		glm::mat4 m_CaptureProjection;
		std::vector<glm::mat4> m_CaptureViews;


	};
}