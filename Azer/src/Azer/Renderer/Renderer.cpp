#include "azpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Azer {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthoGraphicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Azer::Ref<VertexArray>& vertexArray, const Azer::Ref<Shader> shader, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4(m_SceneData->ViewProjectionMatrix, "u_ViewProjection");
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4(transform, "u_Transform");
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}