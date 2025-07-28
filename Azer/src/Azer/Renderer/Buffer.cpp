#include "azpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Azer {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None :
			return nullptr;
		case RendererAPI::API::OpenGL :
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}
}