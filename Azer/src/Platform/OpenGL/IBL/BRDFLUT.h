#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/VertexArray.h>

namespace Azer {

	class BRDFLUT
	{
	public:
		BRDFLUT();
		~BRDFLUT();

		void BakeRender();
		void Render();

		inline uint32_t GetRendererID() const { return m_brdfLUTTexture; }

	private:
		Ref<Shader> m_BRDFLUT_shader;
		Ref<FrameBufferRenderBuffer> m_fbrb;
		Ref<VertexArray> m_VertexArray;

		uint32_t m_brdfLUTTexture;

	};
}