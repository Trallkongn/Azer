#include "azpch.h"
#include "Renderer3D.h"
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Renderer/Renderer.h>

namespace Azer {

	struct Renderer3DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> renderer3DShader;
		Ref<IndexBuffer> ebo;
		Ref<IndexBuffer> edgeEbo;
	};

	static Renderer3DStorage* s_Data_3D;

	void Renderer3D::Init() {
		AZ_PROFILE_FUNCTION();

		s_Data_3D = new Renderer3DStorage;

		(s_Data_3D->vertexArray) = VertexArray::Create();
		
		s_Data_3D->vertexArray->Bind();

		(s_Data_3D->renderer3DShader) = Shader::Create("assets/shaders/Renderer3D.glsl");

		s_Data_3D->renderer3DShader->Bind();

		float vertices[8 * 3] = {
			-0.5, -0.5, -0.5, //0
			 0.5, -0.5, -0.5, //1
			 0.5,  0.5, -0.5, //2
			-0.5,  0.5, -0.5, //3
			-0.5, -0.5,  0.5, //4
			 0.5, -0.5,  0.5, //5
			 0.5,  0.5,  0.5, //6
			-0.5,  0.5,  0.5  //7
		};

		uint32_t indices[6 * 6] = {
			0,1,2,2,3,0,
			5,4,7,7,6,5,
			4,0,3,3,7,4,
			1,5,6,6,2,1,
			3,2,6,6,7,3,
			4,5,1,1,0,4
		};

		uint32_t edgeIndices[24] = {
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,4, 1,5, 2,6, 3,7
		};

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->Bind();

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" }
		};
		vbo->SetLayout(layout);

		(s_Data_3D->vertexArray)->AddVertexBuffer(vbo);

		s_Data_3D->ebo = IndexBuffer::Create(indices, sizeof(indices));
		s_Data_3D->edgeEbo = IndexBuffer::Create(edgeIndices, sizeof(edgeIndices));

		s_Data_3D->ebo->Bind();
		(s_Data_3D->vertexArray)->SetIndexBuffer(s_Data_3D->ebo);
	}
	
	void Renderer3D::BeginScene(PerspectiveGraphicCamera& camera) {
		AZ_PROFILE_FUNCTION();

		(s_Data_3D->renderer3DShader)->Bind();
		(s_Data_3D->renderer3DShader)->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer3D::EndScene() {
		AZ_PROFILE_FUNCTION();
		// Code to finalize rendering the current scene
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, glm::vec4& color)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data_3D->renderer3DShader)->Bind();
		(s_Data_3D->renderer3DShader)->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));
		(s_Data_3D->renderer3DShader)->SetMat4("u_Transform", transform);

		s_Data_3D->ebo->Bind();
		s_Data_3D->vertexArray->SetIndexBuffer(s_Data_3D->ebo);
		s_Data_3D->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data_3D->vertexArray);
	}

	void Renderer3D::DrawCubeWithEdge(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec4& edgeColor)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data_3D->renderer3DShader)->Bind();
		(s_Data_3D->renderer3DShader)->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));
		(s_Data_3D->renderer3DShader)->SetMat4("u_Transform", transform);

		s_Data_3D->ebo->Bind();
		s_Data_3D->vertexArray->SetIndexBuffer(s_Data_3D->ebo);
		s_Data_3D->vertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data_3D->vertexArray);

		(s_Data_3D->renderer3DShader)->SetFloat4("u_Color", edgeColor);

		s_Data_3D->edgeEbo->Bind();
		s_Data_3D->vertexArray->SetIndexBuffer(s_Data_3D->edgeEbo);
		s_Data_3D->vertexArray->Bind();
		RenderCommand::DrawLine(s_Data_3D->vertexArray);
	}

	void Renderer3D::Shutdown() {
		// Cleanup code for the 3D renderer
		AZ_PROFILE_FUNCTION();
	}
} // namespace Azer