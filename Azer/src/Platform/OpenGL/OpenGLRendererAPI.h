#pragma once
#include "Azer/Renderer/RendererAPI.h"

namespace Azer {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void DrawIndexed(const Azer::Ref<VertexArray> vertexArray) override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
}