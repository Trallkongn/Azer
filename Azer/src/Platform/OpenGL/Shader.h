#pragma once
#include "Azer/Core.h"
#include "glm.hpp"

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

namespace Azer {
	class AZER_API Shader
	{
	public:
		Shader(const std::string& FilePath);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1iv(const std::string& name, int* value, int count);
		void SetUniform1f(const std::string& name, int value);
		void SetUniform4i(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4fv(const std::string& name, const glm::mat4& matrix);
	private:
		ShaderProgramSource ParseShder(const std::string& FilePath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
	private:
		std::string m_FilePath;
		unsigned int m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}
