#pragma once

#include "Azer/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO:Remove!
typedef unsigned int GLenum;

namespace Azer {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, std::string& fragmentSrc);
		~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		void SetUniformInt(int value, const std::string& name) const;
		void SetUniformInt2(const glm::vec2& values, const std::string& name) const;
		void SetUniformInt3(const glm::vec3& values, const std::string& name) const;
		void SetUniformInt4(const glm::vec4& values, const std::string& name) const;

		void SetUniformFloat(float value, const std::string& name) const;
		void SetUniformFloat2(const glm::vec2& values, const std::string& name) const;
		void SetUniformFloat3(const glm::vec3& values, const std::string& name) const;
		void SetUniformFloat4(const glm::vec4& values, const std::string& name) const;

		void SetUniformMat3(const glm::mat3& matrix, const std::string& name) const;
		void SetUniformMat4(const glm::mat4& matrix, const std::string& name) const;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID;
	};
}