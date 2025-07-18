#pragma once

#include <memory>
#include "Azer/Renderer/Buffer.h"

namespace Azer {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(Azer::Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(Azer::Ref<IndexBuffer> indexBuffer) = 0;

		virtual const std::vector<Azer::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Azer::Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}