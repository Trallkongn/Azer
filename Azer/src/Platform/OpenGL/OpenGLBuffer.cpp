#include "azpch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"


// VertexBuffer //////////////////////////////////////////////////
Azer::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
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
Azer::OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indeces, uint32_t count)
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
