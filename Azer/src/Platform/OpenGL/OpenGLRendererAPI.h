#pragma once
#include "Azer/Renderer/RendererAPI.h"

namespace Azer {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		// Í¨¹ý RendererAPI ¼Ì³Ð
		void SetClearColor(const glm::vec4& color) override;

		void Clear() override;

		void DrawIndexed(const std::shared_ptr<VertexArray> vertexArray) override;

	};
}