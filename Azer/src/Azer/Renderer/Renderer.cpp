#include "azpch.h"
#include "Renderer.h"

namespace Azer {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthoGraphicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader> shader)
	{
		shader->Bind();
		shader->SetUniformMat4(m_SceneData->ViewProjectionMatrix, "u_ViewProjection");
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}