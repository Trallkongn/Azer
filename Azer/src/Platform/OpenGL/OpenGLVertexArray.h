#pragma once

#include <vector>
#include "Azer/Renderer/VertexArray.h"

namespace Azer {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		// Í¨¹ý VertexArray ¼Ì³Ð
		void Bind() const override;
		void UnBind() const override;
		void AddVertexBuffer(Azer::Ref<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(Azer::Ref<IndexBuffer> indexBuffer) override;
		inline const std::vector<Azer::Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		inline const Azer::Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;

		std::vector<Azer::Ref<VertexBuffer>> m_VertexBuffers;
		Azer::Ref<IndexBuffer> m_IndexBuffer;
	};
}