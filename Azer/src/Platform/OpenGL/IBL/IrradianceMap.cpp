#include "azpch.h"
#include "IrradianceMap.h"
#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>

namespace Azer {

	IrradianceMap::IrradianceMap(uint32_t width, uint32_t height, Ref<CubeMap> envCubemap, Ref<VertexArray> vertexArray)
		: m_Width(width), m_Height(height), 
		m_EnvCubemap(envCubemap), 
		m_VertexArray(vertexArray)
	{
		m_CubeMap = CubeMap::Create(m_Width,m_Height);
		m_fbrb = FrameBufferRenderBuffer::Create(m_Width, m_Height);
		m_IrradianceMapShader = Shader::Create("assets/shaders/PBR/IrradianceConvolutionShader.glsl");
	}

	IrradianceMap::~IrradianceMap()
	{

	}

	void IrradianceMap::BakeRender()
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		// ÀëÆÁäÖÈ¾
		m_IrradianceMapShader->Bind();
		m_IrradianceMapShader->SetInt("environmentMap", 0);
		m_EnvCubemap->Bind();
		m_IrradianceMapShader->SetMat4("u_Projection", m_CaptureProjection);
		m_fbrb->Bind();
		for (uint32_t i = 0; i < 6; i++)
		{
			m_IrradianceMapShader->SetMat4("u_View", m_CaptureViews[i]);
			RenderCommand::FramebufferTexture2D(i, m_CubeMap->GetRendererID());
			RenderCommand::SetViewport(0, 0, m_Width, m_Height);
			RenderCommand::Clear();
			RenderCommand::DrawArray(m_VertexArray, 36);
		}
		m_fbrb->UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}

	void IrradianceMap::Render()
	{
		
	}
}