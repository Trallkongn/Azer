#include "azpch.h"
#include "Renderer2D.h"

#include "Renderer.h"

#include "VertexArray.h"
#include "Shader.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Azer {

	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		AZ_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage;
		s_Data->vertexArray = VertexArray::Create();

		float vertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,//0
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,//1
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,//2
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f //3
		};

		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{Azer::ShaderDataType::Float3, "a_Position"},
			{Azer::ShaderDataType::Float2, "a_TexCoord"}
		};

		vb->SetLayout(layout);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, 6);

		s_Data->vertexArray->AddVertexBuffer(vb);
		s_Data->vertexArray->SetIndexBuffer(ib);

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		AZ_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		AZ_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color)
	{
		AZ_PROFILE_FUNCTION();

		Renderer2D::DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->SetFloat4("u_Color",color);
		(s_Data->TextureShader)->SetFloat("u_TilingFactor", 1.0f);

		// Bind white texture here
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		(s_Data->TextureShader)->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		AZ_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->SetInt("u_Texture", 0);
		(s_Data->TextureShader)->SetFloat4("u_Color", tintColor);
		(s_Data->TextureShader)->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		(s_Data->TextureShader)->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, glm::vec4& color)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, glm::vec4& color)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->SetFloat4("u_Color", color);
		(s_Data->TextureShader)->SetFloat("u_TilingFactor", 1.0f);

		// Bind white texture here
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		(s_Data->TextureShader)->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->SetInt("u_Texture", 0);
		(s_Data->TextureShader)->SetFloat4("u_Color", tintColor);
		(s_Data->TextureShader)->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		(s_Data->TextureShader)->SetMat4("u_Transform", transform);

		s_Data->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->vertexArray);
	}
}

