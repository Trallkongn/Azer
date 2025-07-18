#pragma once
#include "Azer/Renderer/RendererAPI.h"

namespace Azer {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		// Í¨¹ý RendererAPI ¼Ì³Ð
		void Init() override;

		void SetClearColor(const glm::vec4& color) override;

		void Clear() override;

		void DrawIndexed(const Azer::Ref<VertexArray> vertexArray) override;

	};
}