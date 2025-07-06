#pragma once

#include "Azer/Renderer/Buffer.h"
#include "glad/glad.h"

namespace Azer {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		// Extended by VertexBuffer
		void Bind() const override;
		void UnBind() const override;

		inline const BufferLayout& GetLayout() const override { return m_Layout; }
		inline void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indeces, uint32_t count);
		~OpenGLIndexBuffer();

		// Extended by IndexBuffer
		void Bind() const override;
		void UnBind() const override;
		uint32_t GetCount() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}