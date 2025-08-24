#pragma once

#include "Azer/Renderer/Buffer.h"
#include "glad/glad.h"

namespace Azer {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size);
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
		OpenGLIndexBuffer(void* indeces, uint32_t count);
		~OpenGLIndexBuffer();

		// Extended by IndexBuffer
		void Bind() const override;
		void UnBind() const override;
		uint32_t GetCount() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer();
		~OpenGLUniformBuffer();

		// 通过 UniformBuffer 继承
		void Bind() const override;

		void UnBind() const override;

	private:
		uint32_t m_RendererID;
	};

	class OpenGLFrameBufferRenderBuffer : public FrameBufferRenderBuffer
	{
	public:
		OpenGLFrameBufferRenderBuffer(uint32_t width = 512, uint32_t height = 512);
		~OpenGLFrameBufferRenderBuffer();

		// 通过 FrameBufferRenderBuffer 继承
		void Bind() const override;

		void UnBind() const override;

		uint32_t GetRendererID() const { return m_FrameBuffer; }

		void AttachColorTexture(uint32_t textureID, GLenum target = GL_TEXTURE_2D, int level = 0) override;

	private:
		uint32_t m_FrameBuffer;
		uint32_t m_RenderBuffer;

		uint32_t m_Width;
		uint32_t m_Height;
	};
}