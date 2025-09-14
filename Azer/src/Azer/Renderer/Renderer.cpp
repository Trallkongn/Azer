#include "azpch.h"

#include "Renderer.h"
#include "Renderer2D.h"
#include <Azer/Renderer3D/Renderer3D.h>

namespace Azer {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		AZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();
		
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(GraphicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		Renderer2D::EndScene();
		Renderer3D::EndScene();
	}

	void Renderer::Submit(const Azer::Ref<VertexArray>& vertexArray, const Azer::Ref<Shader> shader, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->UnBind();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
}