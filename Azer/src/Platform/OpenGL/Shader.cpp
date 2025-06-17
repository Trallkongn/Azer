#include "azpch.h"
#include "Shader.h"
#include "glad/glad.h"

#include <fstream>


Azer::Shader::Shader(const std::string& FilePath)
	:m_FilePath(FilePath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShder(m_FilePath);

	std::cout << "\nShader source:\n" << '\n';
	std::cout << "VertexShader:\n" << source.VertexShader << '\n';
	std::cout << "FragmentShader:\n" << source.FragmentShader << '\n';

	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
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

void Azer::Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Azer::Shader::SetUniform1iv(const std::string& name, int* value, int count)
{
	glUniform1iv(GetUniformLocation(name), count, value);
}

void Azer::Shader::SetUniform1f(const std::string& name, int value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Azer::Shader::SetUniform4i(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
}

void Azer::Shader::SetUniformMat4fv(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name),1,GL_FALSE, &matrix[0][0]);
}

ShaderProgramSource Azer::Shader::ParseShder(const std::string& FilePath)
{
	std::ifstream stream(FilePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Azer::Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id =  glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id,GL_COMPILE_STATUS,&result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout
			<< "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!"
			<< std::endl;
		std::cout << message << std::endl;
		return 0;
	}
	return id;
}

unsigned int Azer::Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int Azer::Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.count(name)) return m_UniformLocationCache[name];
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) std::cout << "[Warning] uniform ' " << name << " ' doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}
