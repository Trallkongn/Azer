#include "azpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace Azer {

	void OpenGLRendererAPI::Init()
	{
		AZ_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
	{
		glViewport(x, y, w, h);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r,color.g,color.b,color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Azer::Ref<VertexArray> vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::DrawLine(const Azer::Ref<VertexArray> vertexArray)
	{
		glDrawElements(GL_LINES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::DrawArray(const Azer::Ref<VertexArray> vertexArray, uint32_t count)
	{
		vertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, count);
		glBindVertexArray(0);
	}

	void OpenGLRendererAPI::FramebufferTexture2D(uint32_t index, uint32_t rendererID, int miplevels)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
			rendererID, miplevels);
	}
}