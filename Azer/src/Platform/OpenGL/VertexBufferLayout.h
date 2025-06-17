#pragma once

#include "Azer/Core.h"
#include "glad/glad.h"
#include <assert.h>
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char isNormalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		assert(false);
		return 0;
	}
};

namespace Azer {
	class AZER_API VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			:m_Stride(0){ }

		template<typename T>
		void Push(unsigned int count) { assert(false); }

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({GL_FLOAT,count,GL_FALSE});
			m_Stride += VertexBufferElement::GetSizeOfType(GL_FALSE) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
			m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	};
}
