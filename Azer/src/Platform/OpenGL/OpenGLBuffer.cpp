#include "azpch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"


// VertexBuffer //////////////////////////////////////////////////
Azer::OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size)
	: m_RendererID(0)
{
	AZ_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Azer::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	AZ_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_RendererID);
}

void Azer::OpenGLVertexBuffer::Bind() const
{
	AZ_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void Azer::OpenGLVertexBuffer::UnBind() const
{
	AZ_PROFILE_FUNCTION();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// IndexBuffer //////////////////////////////////////////////////
Azer::OpenGLIndexBuffer::OpenGLIndexBuffer(void* indeces, uint32_t count)
	: m_RendererID(0), m_Count(count)
{
	AZ_PROFILE_FUNCTION();

	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indeces, GL_STATIC_DRAW);
}

Azer::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	AZ_PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_RendererID);
}

void Azer::OpenGLIndexBuffer::Bind() const
{
	AZ_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void Azer::OpenGLIndexBuffer::UnBind() const
{
	AZ_PROFILE_FUNCTION();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t Azer::OpenGLIndexBuffer::GetCount() const
{
	return m_Count;
}

Azer::OpenGLUniformBuffer::OpenGLUniformBuffer()
{
	/*glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_RendererID);*/
}

Azer::OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void Azer::OpenGLUniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
}

void Azer::OpenGLUniformBuffer::UnBind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Azer::OpenGLFrameBufferRenderBuffer::OpenGLFrameBufferRenderBuffer(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height), m_FrameBuffer(0), m_RenderBuffer(0)
{
	AZ_CORE_INFO("start create framebuffer");
	glGenFramebuffers(1, &m_FrameBuffer);
	glGenRenderbuffers(1, &m_RenderBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

	// 创建深度 RBO
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	AZ_CORE_INFO("created framebuffer");
}

Azer::OpenGLFrameBufferRenderBuffer::~OpenGLFrameBufferRenderBuffer()
{
	glDeleteFramebuffers(1, &m_FrameBuffer);
	glDeleteRenderbuffers(1, &m_RenderBuffer);
}

void Azer::OpenGLFrameBufferRenderBuffer::AttachColorTexture(uint32_t textureID, GLenum target, int level)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, textureID, level);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	AZ_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer not complete!");
}

void Azer::OpenGLFrameBufferRenderBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void Azer::OpenGLFrameBufferRenderBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
