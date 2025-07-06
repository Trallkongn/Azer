#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Azer {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void SetUniformMat4(const glm::mat4& matrix, const std::string& name) const;
	private:
		unsigned int m_RendererID;
	};
}