#include "azpch.h"
#include "OpenGLBRDFLUT.h"
#include <Azer/Renderer/Renderer.h>
#include <glad/glad.h>


namespace Azer {

	OpenGLBRDFLUT::OpenGLBRDFLUT(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		m_BRDFLUT_shader = Shader::Create("assets/shaders/PBR/BRDF_Shader.glsl");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_Width, m_Height, 0, GL_RG, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_fbrb = FrameBufferRenderBuffer::Create(512, 512);

		m_VertexArray = VertexArray::Create();

		float quadVertices[] = {
			// positions   // texCoords
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		Ref<VertexBuffer> vbo = VertexBuffer::Create(quadVertices, sizeof(quadVertices));

		BufferLayout layout = {
			{ ShaderDataType::Float2, "a_Pos"},
			{ ShaderDataType::Float2, "a_TexCoords"}
		};

		vbo->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vbo);
	}

	OpenGLBRDFLUT::~OpenGLBRDFLUT()
	{

	}

	void OpenGLBRDFLUT::BakeRender()
	{
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		m_BRDFLUT_shader->Bind();
		glBindTextureUnit(0, m_RendererID);
		m_fbrb->Bind();
		RenderCommand::SetViewport(0, 0, 512, 512);
		RenderCommand::Clear();

		m_VertexArray->Bind();
		RenderCommand::DrawArray(m_VertexArray, 6);
		m_VertexArray->UnBind();

		m_fbrb->UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
}