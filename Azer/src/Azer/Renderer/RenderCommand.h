#pragma once
#include "RendererAPI.h"

namespace Azer
{
	class RenderCommand
	{
	public:
		inline static void Init()
		{
			AZ_PROFILE_FUNCTION();

			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
		{
			s_RendererAPI->SetViewport(x, y, w, h);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Azer::Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawLine(const Azer::Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawLine(vertexArray);
		}
		inline static void DrawLine(const Azer::Ref<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawLine(vertexArray, count);
		}
		inline static void DrawPoints(const Azer::Ref<VertexArray> vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawPoints(vertexArray, count);
		}
		inline static void DrawArray(const Azer::Ref<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawArray(vertexArray, count);
		}

		inline static void FramebufferTexture2D(uint32_t index, uint32_t rendererID, int miplevels = 0)
		{
			s_RendererAPI->FramebufferTexture2D(index, rendererID, miplevels);
		}

		inline static void CullFace(bool statu = true)
		{
			s_RendererAPI->CullFace(statu);
		}
		inline static void DeepTest(bool statu = true)
		{
			s_RendererAPI->DeepTest(statu);
		}

		inline static void BindTexUnit(uint32_t TexId, int slot = 0)
		{
			s_RendererAPI->BindTexUnit(TexId, slot);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}