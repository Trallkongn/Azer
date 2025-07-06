#include "azpch.h"
#include "Shader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

Azer::Shader::Shader(const std::string& vertexSrc, std::string& fragmentSrc)
	: m_RendererID(0)
{
	const GLchar* Source = nullptr;

	// deal with vertexShader
	unsigned int vertexShader =  glCreateShader(GL_VERTEX_SHADER);
	Source = (const GLchar*)vertexSrc.c_str();
	glShaderSource(vertexShader, 1, &Source, 0);
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		AZ_CORE_ERROR("Vertex shader compilation failure!");
		AZ_CORE_ERROR("{0}", infoLog.data());
		AZ_CORE_ASSERT(false, "\n Please check errors!");
		return;
	}

	// deal with fragmentShader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	Source = (const GLchar*)fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &Source, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		AZ_CORE_ERROR("Fragment shader compilation failure!");
		AZ_CORE_ERROR("{0}", infoLog.data());
		AZ_CORE_ASSERT(false, "\n Please check errors!");
		return;
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		AZ_CORE_ERROR("Shader program link failure!");
		AZ_CORE_ERROR("{0}", infoLog.data());
		AZ_CORE_ASSERT(false, "\n Please check errors!");
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	m_RendererID = program;
}

Azer::Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Azer::Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Azer::Shader::UnBind() const
{
	glUseProgram(0);
}

void Azer::Shader::SetUniformMat4(const glm::mat4& matrix, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
