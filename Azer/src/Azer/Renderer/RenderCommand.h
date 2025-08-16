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
	private:
		static RendererAPI* s_RendererAPI;
	};
}