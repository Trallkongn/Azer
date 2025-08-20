#include "azpch.h"
#include "EnvCubemap.h"
#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>

namespace Azer {

	EnvCubeMap::EnvCubeMap(uint32_t width, uint32_t height, const std::string& HDRpath, Ref<VertexArray> vertexArray)
		: m_Width(width), m_Height(height), m_HDRpath(HDRpath),
		m_VertexArray(vertexArray)
	{
		AZ_CORE_INFO("HDRpath -> {}", HDRpath);
		m_HDR_env_shader = Shader::Create("assets/shaders/PBR/HDRtoCubeMap.glsl");
		m_SkyBoxShader = Shader::Create("assets/shaders/PBR/SkyBoxShader.glsl");
		m_CubeMap = CubeMap::Create( m_Width,m_Height );
		m_fbrb = FrameBufferRenderBuffer::Create(m_Width,m_Height);
		m_HDR_texture = Texture2D::Create(HDRpath, true);
	}

	EnvCubeMap::~EnvCubeMap()
	{

	}

	void EnvCubeMap::BakeRender()
	{
		m_HDR_env_shader->Bind();
		m_HDR_env_shader->SetInt("equirectangularMap", 0); // 绑定到纹理单元 0
		m_HDR_texture->Bind();
		m_HDR_env_shader->SetMat4("u_Projection", m_CaptureProjection);

		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_DEPTH_TEST);


		m_fbrb->Bind();

		for (unsigned int i = 0; i < 6; ++i)
		{
			m_HDR_env_shader->SetMat4("u_View", m_CaptureViews[i]);

			m_fbrb->AttachColorTexture(m_CubeMap->GetRendererID(),
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

			RenderCommand::SetViewport(0, 0, m_Width, m_Height);
			RenderCommand::Clear();
			RenderCommand::DrawArray(m_VertexArray, 36);
		}

		m_fbrb->UnBind();

		// 生成mipmap
		m_CubeMap->Bind(0);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}

	void EnvCubeMap::Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		glDisable(GL_CULL_FACE);

		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("environmentMap", 0); // 绑定到纹理单元 0
		
		m_CubeMap->Bind(0);

		m_SkyBoxShader->SetMat4("u_Projection", projectionMatrix);
		m_SkyBoxShader->SetMat4("u_View", glm::mat3(viewMatrix));

		RenderCommand::DrawArray(m_VertexArray, 36);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
	}
}