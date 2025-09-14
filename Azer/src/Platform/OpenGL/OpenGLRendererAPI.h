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
		void DrawLine(const Azer::Ref<VertexArray> vertexArray) override;
		void DrawLine(const Azer::Ref<VertexArray> vertexArray, uint32_t count) override;
		void DrawArray(const Azer::Ref<VertexArray> vertexArray, uint32_t count) override;
		void DrawPoints(const Azer::Ref<VertexArray> vertexArray, uint32_t count) override;

		void FramebufferTexture2D(uint32_t index, uint32_t rendererID, int miplevels = 0) override;

		void CullFace(bool statu = true) override;
		void DeepTest(bool statu = true) override;

		void BindTexUnit(uint32_t TexId, int slot = 0) override;
	};
}