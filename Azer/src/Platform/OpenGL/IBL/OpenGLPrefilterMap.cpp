#include "azpch.h"
#include "OpenGLPrefilterMap.h"
#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>

namespace Azer {

	OpenGLPrefilterMap::OpenGLPrefilterMap(uint32_t miplevels, uint32_t width, uint32_t height,
		uint32_t skyboxID,
		Ref<VertexArray> vertexArray,
		const glm::mat4& captureProjection,
		const std::vector<glm::mat4>& captureViews
	)
		: m_Miplevels(miplevels), m_Width(width), m_Height(height),
		m_VertexArray(vertexArray),in_skyboxID(in_skyboxID),
		m_CaptureProjection(captureProjection),
		m_CaptureViews(captureViews)
	{
		m_CubeMap = CubeMap::Create(m_Miplevels);
		m_RendererID = m_CubeMap->GetRendererID();

		std::string samplerShaderPath = "assets/shaders/PBR/PrefilterMapShader.glsl";
		m_PrefilterMapShader = Shader::Create(samplerShaderPath);
		AZ_CORE_INFO("PrefilterMapShader shader path -> {}", samplerShaderPath);

		m_fbrb = FrameBufferRenderBuffer::Create(m_Width, m_Height);
	}

	OpenGLPrefilterMap::~OpenGLPrefilterMap()
	{

	}

	void OpenGLPrefilterMap::BakeRender()
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		m_PrefilterMapShader->Bind();
		m_PrefilterMapShader->SetInt("environmentMap", 0);
		
		glBindTextureUnit(0, in_skyboxID);

		m_PrefilterMapShader->SetMat4("u_Projection", m_CaptureProjection);

		for (unsigned int mip = 0; mip < m_Miplevels; ++mip)
		{
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);

			m_fbrb->Bind();
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(m_Miplevels - 1);
			m_PrefilterMapShader->SetFloat("roughness", roughness);

			for (unsigned int i = 0; i < 6; ++i)
			{
				m_PrefilterMapShader->SetMat4("u_View", m_CaptureViews[i]);
				RenderCommand::FramebufferTexture2D(i, m_CubeMap->GetRendererID(), mip);
				RenderCommand::SetViewport(0, 0, m_Width, m_Height);
				RenderCommand::Clear();

				m_VertexArray->Bind();
				RenderCommand::DrawArray(m_VertexArray, 36);
			}
		}
		m_fbrb->UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
}