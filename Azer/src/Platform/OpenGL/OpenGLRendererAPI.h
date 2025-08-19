#pragma once
#include "Azer/Renderer/RendererAPI.h"

namespace Azer {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		// Í¨¹ý RendererAPI ¼Ì³Ð
		void Init() override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;

		void SetClearColor(const glm::vec4& color) override;

		void Clear() override;

		void DrawIndexed(const Azer::Ref<VertexArray> vertexArray) override;

	};
}