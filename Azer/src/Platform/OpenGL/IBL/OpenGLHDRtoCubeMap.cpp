#include "azpch.h"
#include "OpenGLHDRtoCubeMap.h"

#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>

namespace Azer {

	OpenGLHDRtoCubeMap::OpenGLHDRtoCubeMap(uint32_t width, uint32_t height, 
		const std::string& HDRpath, Ref<VertexArray> vertexArray,
		const glm::mat4& captureProjection,
		const std::vector<glm::mat4>& captureViews)
		:
		m_Width(width),
		m_Height(height),
		m_HDRpath(HDRpath),
		m_VertexArray(vertexArray),
		m_CaptureProjection(captureProjection),
		m_CaptureViews(captureViews)
	{
		AZ_CORE_INFO("HDRtoCubeMap logs:");
		AZ_CORE_INFO("HDRpath -> {}", HDRpath);

		std::string samplerShaderPath = "assets/shaders/PBR/HDRtoCubeMap.glsl";
		m_HDRtoCubeMap_shader = Shader::Create(samplerShaderPath);
		AZ_CORE_INFO("HDRtoCubeMap shader path -> {}", samplerShaderPath);

		std::string renderShaderPath = "assets/shaders/PBR/SkyBoxShader.glsl";
		m_SkyBoxShader = Shader::Create(renderShaderPath);
		AZ_CORE_INFO("SkyBox shader path -> {}", renderShaderPath);

		m_CubeMap = CubeMap::Create(m_Width, m_Height);
		m_RendererID = m_CubeMap->GetRendererID();

		m_fbrb = FrameBufferRenderBuffer::Create(m_Width, m_Height);
		m_HDR_texture = Texture2D::Create(HDRpath, true);
	}

	OpenGLHDRtoCubeMap::~OpenGLHDRtoCubeMap()
	{

	}

	void OpenGLHDRtoCubeMap::BakeRender()
	{
		m_HDRtoCubeMap_shader->Bind();
		m_HDRtoCubeMap_shader->SetInt("equirectangularMap", 0); // 绑定到纹理单元 0
		m_HDR_texture->Bind();
		m_HDRtoCubeMap_shader->SetMat4("u_Projection", m_CaptureProjection);

		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_DEPTH_TEST);


		m_fbrb->Bind();

		for (unsigned int i = 0; i < m_CaptureViews.size(); ++i)
		{
			m_HDRtoCubeMap_shader->SetMat4("u_View", m_CaptureViews[i]);

			m_fbrb->AttachColorTexture(m_CubeMap->GetRendererID(),
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			RenderCommand::SetViewport(0, 0, m_Width, m_Height);
			RenderCommand::Clear();

			m_VertexArray->Bind();
			RenderCommand::DrawArray(m_VertexArray, 36);
			m_VertexArray->UnBind();
		}

		m_fbrb->UnBind();

		// 生成mipmap
		m_CubeMap->Bind(0);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);

		m_IsBaked = true;
	}

	void OpenGLHDRtoCubeMap::RenderScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const
	{
		AZ_CORE_ASSERT(m_IsBaked, "Please bake your skybox scene first!");

		glDisable(GL_CULL_FACE);

		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("environmentMap", 0);

		m_CubeMap->Bind(0);

		m_SkyBoxShader->SetMat4("u_Projection", projectionMatrix);
		m_SkyBoxShader->SetMat4("u_View", glm::mat3(viewMatrix));

		m_VertexArray->Bind();
		RenderCommand::DrawArray(m_VertexArray, 36);
		m_VertexArray->UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
	}
}