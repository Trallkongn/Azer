#include "azpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Azer {

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}


	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		AZ_CORE_ASSERT(!IsShaderExsists(name), "Shader already exist!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		AZ_CORE_ASSERT(!IsShaderExsists(name), "Shader already exist!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) // assets/Texture.glsl
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		AZ_CORE_ASSERT(IsShaderExsists(name), "Shader not found!");
		if (!IsShaderExsists(name)) return nullptr;
		return m_Shaders[name];
	}
	bool ShaderLibrary::IsShaderExsists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}