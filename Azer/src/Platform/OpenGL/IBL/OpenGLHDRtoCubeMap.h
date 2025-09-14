#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/IBL.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Texture.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Azer {

	class OpenGLHDRtoCubeMap : public HDRtoCubeMap
	{
	public:
		OpenGLHDRtoCubeMap(uint32_t width, uint32_t height, 
			const std::string& HDRpath, 
			Ref<VertexArray> vertexArray, 
			const glm::mat4& captureProjection, 
			const std::vector<glm::mat4>& captureViews
		);
		~OpenGLHDRtoCubeMap();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual Ref<VertexArray> GetVertexArray() const override { return m_VertexArray; }

		virtual void SetCaptureProjection(const glm::mat4& captureProjection) override { m_CaptureProjection = captureProjection; }
		virtual void SetCaptureViews(const std::vector<glm::mat4>& captureViews) override { m_CaptureViews = captureViews; }

		virtual void BakeRender() override;
		virtual void RenderScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const override;

	private:
		bool m_IsBaked = false;

		uint32_t m_Width;
		uint32_t m_Height;
		std::string m_HDRpath;
		uint32_t m_RendererID;
		Ref<VertexArray> m_VertexArray;

		Ref<Texture2D> m_HDR_texture;
		Ref<CubeMap> m_CubeMap;
		Ref<Shader> m_HDRtoCubeMap_shader;
		Ref<Shader> m_SkyBoxShader;
		Ref<FrameBufferRenderBuffer> m_fbrb;

		glm::mat4 m_CaptureProjection;
		std::vector<glm::mat4> m_CaptureViews;
	};
}
