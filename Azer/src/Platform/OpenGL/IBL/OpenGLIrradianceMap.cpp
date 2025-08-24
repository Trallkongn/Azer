#include "azpch.h"
#include "OpenGLIrradianceMap.h"
#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>

namespace Azer {

	OpenGLIrradianceMap::OpenGLIrradianceMap(uint32_t width, uint32_t height,
		uint32_t skyboxID,
		Ref<VertexArray> vertexArray,
		const glm::mat4& captureProjection,
		const std::vector<glm::mat4>& captureViews
	)
		:
		m_Width(width), m_Height(height), in_skyboxID(skyboxID),
		m_VertexArray(vertexArray),
		m_CaptureProjection(captureProjection),
		m_CaptureViews(captureViews)
	{
		AZ_CORE_INFO("IrradianceMap logs:");
		m_CubeMap = CubeMap::Create(m_Width,m_Height);
		m_RendererID = m_CubeMap->GetRendererID();

		m_fbrb = FrameBufferRenderBuffer::Create(m_Width, m_Height);

		std::string samplerShaderPath = "assets/shaders/PBR/IrradianceConvolutionShader.glsl";
		m_IrradianceMapShader = Shader::Create(samplerShaderPath);
		AZ_CORE_INFO("IrradianceMapShader shader path -> {}", samplerShaderPath);
	}

	OpenGLIrradianceMap::~OpenGLIrradianceMap()
	{

	}

	void OpenGLIrradianceMap::BakeRender()
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		// ÀëÆÁäÖÈ¾
		m_IrradianceMapShader->Bind();
		m_IrradianceMapShader->SetInt("environmentMap", 0);
		glBindTextureUnit(0, in_skyboxID);
		m_IrradianceMapShader->SetMat4("u_Projection", m_CaptureProjection);
		m_fbrb->Bind();
		for (uint32_t i = 0; i < m_CaptureViews.size(); i++)
		{
			m_IrradianceMapShader->SetMat4("u_View", m_CaptureViews[i]);
			RenderCommand::FramebufferTexture2D(i, m_CubeMap->GetRendererID());
			RenderCommand::SetViewport(0, 0, m_Width, m_Height);
			RenderCommand::Clear();

			m_VertexArray->Bind();
			RenderCommand::DrawArray(m_VertexArray, 36);
			m_VertexArray->UnBind();
		}
		m_fbrb->UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
}