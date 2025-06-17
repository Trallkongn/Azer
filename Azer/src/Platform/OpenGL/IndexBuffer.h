#pragma once
#include "Azer/Core.h"

namespace Azer {
	class AZER_API IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();
		void Bind() const;
		void UnBind() const;
	private:
		unsigned int m_RendererID;
		unsigned int count;
	};
}