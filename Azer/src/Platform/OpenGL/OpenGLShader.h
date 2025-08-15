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
		OpenGLShader(const std::string& name, const std::string& vertexSrc, std::string& fragmentSrc);
		~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		inline const std::string& GetName() const override { return m_Name; }

		void SetFloat(const std::string& name, float value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;
		void SetInt(const std::string& name, int value) override;

		void UploadUniformInt(int value, const std::string& name) const;
		void UploadUniformInt2(const glm::vec2& values, const std::string& name) const;
		void UploadUniformInt3(const glm::vec3& values, const std::string& name) const;
		void UploadUniformInt4(const glm::vec4& values, const std::string& name) const;
			 
		void UploadUniformFloat(float value, const std::string& name) const;
		void UploadUniformFloat2(const glm::vec2& values, const std::string& name) const;
		void UploadUniformFloat3(const glm::vec3& values, const std::string& name) const;
		void UploadUniformFloat4(const glm::vec4& values, const std::string& name) const;
			 
		void UploadUniformMat3(const glm::mat3& matrix, const std::string& name) const;
		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name) const;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}