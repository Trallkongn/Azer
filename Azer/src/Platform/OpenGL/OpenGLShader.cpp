#include "azpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include "glm/gtc/type_ptr.hpp"

namespace Azer {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		AZ_CORE_ASSERT(false, "Unknow shader type!");
		return 0;
	}
}

Azer::OpenGLShader::OpenGLShader(const std::string& filepath)
{
	AZ_PROFILE_FUNCTION();

	std::string source = ReadFile(filepath);
	auto shaderSources = PreProcess(source);
	Compile(shaderSources);

	// ExtractName -> assets/Shaders/Texture.glsls
	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind(".");
	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filepath.substr(lastSlash, count);
}

Azer::OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, std::string& fragmentSrc)
	: m_Name(name)
{
	AZ_PROFILE_FUNCTION();

	std::unordered_map<GLenum, std::string> shaderSources;
	shaderSources[GL_VERTEX_SHADER] = vertexSrc;
	shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

	Compile(shaderSources);
}

Azer::OpenGLShader::~OpenGLShader()
{
	AZ_PROFILE_FUNCTION();

	glDeleteProgram(m_RendererID);
}

std::string Azer::OpenGLShader::ReadFile(const std::string& filepath)
{
	AZ_PROFILE_FUNCTION();

	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		AZ_CORE_ERROR("Could not open file '{0}'", filepath);
	}
	return result;
}

std::unordered_map<GLenum, std::string> Azer::OpenGLShader::PreProcess(const std::string& source)
{
	AZ_PROFILE_FUNCTION();

	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos); // —∞’“––Œ≤
		AZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		AZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified!");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos == std::string::npos ? pos : pos - nextLinePos);
	}

	return shaderSources;
}

void Azer::OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
{
	AZ_PROFILE_FUNCTION();

	GLuint program = glCreateProgram();
	AZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now!");
	std::array<GLuint, 2> glShaderIDs;
	int index = 0;
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;
		uint32_t shader = glCreateShader(type);
		const GLchar* Source = (const GLchar*)source.c_str();
		glShaderSource(shader, 1, &Source, 0);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);

			if(type == GL_VERTEX_SHADER) AZ_CORE_ERROR("Vertex shader compilation failure!");
			if(type == GL_FRAGMENT_SHADER) AZ_CORE_ERROR("Fragment shader compilation failure!");
			
			AZ_CORE_ERROR("{0}", infoLog.data());
			AZ_CORE_ASSERT(false, "\n Please check errors!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs[index++] = shader;
	}

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
		for (auto& shaderID : glShaderIDs) glDeleteShader(shaderID);
		glDeleteProgram(program);

		AZ_CORE_ERROR("Shader program link failure!");
		AZ_CORE_ERROR("{0}", infoLog.data());
		AZ_CORE_ASSERT(false, "\n Please check errors!");
		return;
	}

	m_RendererID = program;

	for (auto& id : glShaderIDs) glDetachShader(program, id);
}

void Azer::OpenGLShader::Bind() const
{
	AZ_PROFILE_FUNCTION();

	glUseProgram(m_RendererID);
}

void Azer::OpenGLShader::UnBind() const
{
	AZ_PROFILE_FUNCTION();

	glUseProgram(0);
}

void Azer::OpenGLShader::SetFloat(const std::string& name, float value)
{
	AZ_PROFILE_FUNCTION();

	UploadUniformFloat(value, name);
}

void Azer::OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
{
	AZ_PROFILE_FUNCTION();

	UploadUniformFloat2(value, name);
}

void Azer::OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
	AZ_PROFILE_FUNCTION();

	UploadUniformFloat3(value, name);
}

void Azer::OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
	AZ_PROFILE_FUNCTION();

	UploadUniformFloat4(value, name);
}

void Azer::OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
	AZ_PROFILE_FUNCTION();

	UploadUniformMat4(value, name);
}

void Azer::OpenGLShader::SetInt(const std::string& name, int value)
{
	AZ_PROFILE_FUNCTION();

	UploadUniformInt(value, name);
}

void Azer::OpenGLShader::UploadUniformInt(int value, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
}

void Azer::OpenGLShader::UploadUniformInt2(const glm::vec2& values, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2i(location, values.x, values.y);
}

void Azer::OpenGLShader::UploadUniformInt3(const glm::vec3& values, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3i(location, values.x, values.y, values.z);
}

void Azer::OpenGLShader::UploadUniformInt4(const glm::vec4& values, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4i(location, values.x, values.y, values.z, values.w);
}

void Azer::OpenGLShader::UploadUniformFloat(float value, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
}

void Azer::OpenGLShader::UploadUniformFloat2(const glm::vec2& values, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, values.x, values.y);
}

void Azer::OpenGLShader::UploadUniformFloat3(const glm::vec3& values, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, values.x, values.y, values.z);
}

void Azer::OpenGLShader::UploadUniformFloat4(const glm::vec4& values, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, values.x, values.y, values.z, values.w);
}

void Azer::OpenGLShader::UploadUniformMat3(const glm::mat3& matrix, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Azer::OpenGLShader::UploadUniformMat4(const glm::mat4& matrix, const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


