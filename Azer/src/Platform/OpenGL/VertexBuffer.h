#pragma once
#include "Azer/Core.h"

namespace Azer {
	class AZER_API VertexBuffer
	{
	public:
		VertexBuffer(const void* data,unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

	private:
		unsigned int m_RendererID;
	};
}
