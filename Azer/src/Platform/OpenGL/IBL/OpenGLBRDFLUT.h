#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/IBL.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/VertexArray.h>

namespace Azer {

	class OpenGLBRDFLUT : public BRDFLUT
	{
	public:
		OpenGLBRDFLUT(uint32_t width, uint32_t height);
		~OpenGLBRDFLUT();

		void BakeRender() override;

		inline uint32_t GetRendererID() const override { return m_RendererID; }
		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }

	private:
		Ref<Shader> m_BRDFLUT_shader;
		Ref<FrameBufferRenderBuffer> m_fbrb;
		Ref<VertexArray> m_VertexArray;

		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t m_RendererID;




	};
}