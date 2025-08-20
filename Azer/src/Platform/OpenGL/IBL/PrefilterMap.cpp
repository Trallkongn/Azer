#include "azpch.h"
#include "PrefilterMap.h"
#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>

namespace Azer {

	PrefilterMap::PrefilterMap(uint32_t width, uint32_t height, int miplevels, Ref<CubeMap> environmentMap, Ref<VertexArray> vertexArray)
		: m_Miplevels(miplevels), m_Width(width), m_Height(height),
		m_VertexArray(vertexArray),m_EnvironmentMap(environmentMap)
	{
		m_CubeMap = CubeMap::Create(miplevels);
		m_PrefilterMapShader = Shader::Create("assets/shaders/PBR/PrefilterMapShader.glsl");
		m_fbrb = FrameBufferRenderBuffer::Create(m_Width, m_Height);

		
	}

	PrefilterMap::~PrefilterMap()
	{

	}

	void PrefilterMap::BakeRender()
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		m_PrefilterMapShader->Bind();
		m_PrefilterMapShader->SetInt("environmentMap", 0);
		m_EnvironmentMap->Bind();

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

	void PrefilterMap::Render()
	{
		
	}
}