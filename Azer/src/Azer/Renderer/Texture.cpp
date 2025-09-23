#include "azpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Azer {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t channels, const unsigned char* data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height,channels,data);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLCubeMap>(width,height);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<CubeMap> CubeMap::Create(int miplevels)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLCubeMap>(miplevels);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

}
