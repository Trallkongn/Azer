#include "azpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Azer {

	Shader* Shader::Create(const std::string& vertexSrc, std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filepath);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}
}