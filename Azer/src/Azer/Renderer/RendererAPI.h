#pragma once
#include "glm/glm.hpp"
#include <memory>
#include "VertexArray.h"

namespace Azer {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Azer::Ref<VertexArray> vertexArray) = 0;
		virtual void DrawLine(const Azer::Ref<VertexArray> vertexArray) = 0;
		virtual void DrawArray(const Azer::Ref<VertexArray> vertexArray, uint32_t count) = 0;

		virtual void FramebufferTexture2D(uint32_t index, uint32_t rendererID, int miplevels = 0) = 0;

		virtual void CullFace(bool statu = true) = 0;

		virtual void BindTexUnit(uint32_t TexId, int slot = 0) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}