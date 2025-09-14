#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/IBL.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Renderer/Shader.h>

namespace Azer {

	class OpenGLIrradianceMap : public IrradianceMap
	{
	public:
		OpenGLIrradianceMap(uint32_t width, uint32_t height,
			uint32_t skyboxID,
			Ref<VertexArray> vertexArray,
			const glm::mat4& captureProjection,
			const std::vector<glm::mat4>& captureViews
		);

		~OpenGLIrradianceMap();

		void BakeRender() override;

		inline uint32_t GetRendererID() const override { return m_RendererID; }
		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }
		inline void SetCaptureProjection(const glm::mat4& captureProjection) override { m_CaptureProjection = captureProjection; }
		inline void SetCaptureViews(const std::vector<glm::mat4>& captureViews) override { m_CaptureViews = captureViews; }
		inline Ref<VertexArray> GetVertexArray() const override { return m_VertexArray; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t in_skyboxID;
		uint32_t m_RendererID;

		Ref<VertexArray> m_VertexArray;

		Ref<CubeMap> m_CubeMap;
		Ref<FrameBufferRenderBuffer> m_fbrb;
		Ref<Shader> m_IrradianceMapShader;

		glm::mat4 m_CaptureProjection;
		std::vector<glm::mat4> m_CaptureViews;
	};
}