#include "azpch.h"
#include "Renderer3D.h"
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Renderer/Renderer.h>
#include <Azer/Renderer/Texture.h>

namespace Azer {

	struct Renderer3DStorage
	{
		Ref<VertexArray> cubeVao;
		Ref<VertexArray> edgeVao;

		Ref<Shader> renderer3DShader;
		Ref<Shader> texture3DShader;

		Ref<VertexBuffer> vbo;
		Ref<VertexBuffer> edgeVbo;
		Ref<IndexBuffer> ebo;
		Ref<IndexBuffer> edgeEbo;

		Ref<Texture2D> texture1;
	};

	static Renderer3DStorage* s_Data_3D;

	void Renderer3D::Init() {
		AZ_PROFILE_FUNCTION();

		s_Data_3D = new Renderer3DStorage;

		s_Data_3D->texture1 = Texture2D::Create("assets/textures/rusty_metal_1k.jpg");
		//s_Data_3D->texture1 = Texture2D::Create("assets/textures/ike.jpg");

		s_Data_3D->cubeVao = VertexArray::Create();
		s_Data_3D->edgeVao = VertexArray::Create();

		// 初始化着色器
		s_Data_3D->renderer3DShader		= Shader::Create("assets/shaders/Renderer3D.glsl");
		s_Data_3D->texture3DShader		= Shader::Create("assets/shaders/Texture3D.glsl");

		(s_Data_3D->texture3DShader)->Bind();
		(s_Data_3D->texture3DShader)->SetInt("u_TexSampler", 0);

		float vertices[] = {
			// 前面  Z = +0.5
				-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // 0
				 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // 1
				 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // 2
				-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // 3

				// 后面  Z = -0.5
				 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 4
				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 5
				-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // 6
				 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  // 7

				 // 右面  X = +0.5
				  0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // 8
				  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 9
				  0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // 10
				  0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // 11

				  // 左面  X = -0.5
				  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 12
				  -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // 13
				  -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // 14
				  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  // 15

				  // 顶面  Y = +0.5
				  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  // 16
				   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  // 17
				   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // 18
				  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  // 19

				  // 底面  Y = -0.5
				  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 20
				   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 21
				   0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  // 22
				  -0.5f, -0.5f,  0.5f, 0.0f, 1.0f   // 23
		};

		float edgeVertices[3*8] = {
			-0.5, -0.5, -0.5, //0
			 0.5, -0.5, -0.5, //1
			 0.5,  0.5, -0.5, //2
			-0.5,  0.5, -0.5, //3
			-0.5, -0.5,  0.5, //4
			 0.5, -0.5,  0.5, //5
			 0.5,  0.5,  0.5, //6
			-0.5,  0.5,  0.5  //7
		};

		uint32_t indices[] = {
			// 前面 Z = +0.5
			0, 1, 2,
			2, 3, 0,

			// 后面 Z = -0.5
			4, 5, 6,
			6, 7, 4,

			// 右面 X = +0.5
			8, 9, 10,
			10, 11, 8,

			// 左面 X = -0.5
			12, 13, 14,
			14, 15, 12,

			// 顶面 Y = +0.5
			16, 17, 18,
			18, 19, 16,

			// 底面 Y = -0.5
			20, 21, 22,
			22, 23, 20
		};

		uint32_t edgeIndices[3*8] = {
			0,1,1,2,2,3,3,0,
			4,5,5,6,6,7,7,4,
			0,4,1,5,2,6,3,7
		};

		s_Data_3D->vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		s_Data_3D->edgeVbo = VertexBuffer::Create(edgeVertices, sizeof(edgeVertices));
		
		s_Data_3D->vbo->Bind();
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_Tex"}
		};
		s_Data_3D->vbo->SetLayout(layout);

		s_Data_3D->edgeVbo->Bind();
		BufferLayout layout2 = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		s_Data_3D->edgeVbo->SetLayout(layout2);

		s_Data_3D->cubeVao->AddVertexBuffer(s_Data_3D->vbo);
		s_Data_3D->edgeVao->AddVertexBuffer(s_Data_3D->edgeVbo);

		s_Data_3D->ebo = IndexBuffer::Create(indices, IndexBuffer::CountOf(indices));
		s_Data_3D->edgeEbo = IndexBuffer::Create(edgeIndices, IndexBuffer::CountOf(edgeIndices));

		s_Data_3D->cubeVao->SetIndexBuffer(s_Data_3D->ebo);
		s_Data_3D->edgeVao->SetIndexBuffer(s_Data_3D->edgeEbo);
	}
	
	void Renderer3D::BeginScene(PerspectiveGraphicCamera& camera) {
		AZ_PROFILE_FUNCTION();

		(s_Data_3D->renderer3DShader)->Bind();
		(s_Data_3D->renderer3DShader)->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data_3D->texture3DShader->Bind();
		s_Data_3D->texture3DShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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

		s_Data_3D->cubeVao->Bind();
		RenderCommand::DrawIndexed(s_Data_3D->cubeVao);
	}

	void Renderer3D::DrawCubeWithEdge(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec4& edgeColor)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data_3D->renderer3DShader)->Bind();
		(s_Data_3D->renderer3DShader)->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));
		(s_Data_3D->renderer3DShader)->SetMat4("u_Transform", transform);

		s_Data_3D->cubeVao->Bind();
		RenderCommand::DrawIndexed(s_Data_3D->cubeVao);

		s_Data_3D->renderer3DShader->SetFloat4("u_Color", edgeColor);
		s_Data_3D->edgeVao->Bind();
		RenderCommand::DrawLine(s_Data_3D->edgeVao);
	}

	void Renderer3D::DrawTextureCube(const glm::vec3& position, const glm::vec3& size)
	{
		AZ_PROFILE_FUNCTION();

		s_Data_3D->texture3DShader->Bind();
		s_Data_3D->texture1->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));
		(s_Data_3D->texture3DShader)->SetMat4("u_Transform", transform);

		s_Data_3D->cubeVao->Bind();
		RenderCommand::DrawIndexed(s_Data_3D->cubeVao);
	}

	void Renderer3D::Shutdown() {
		// Cleanup code for the 3D renderer
		AZ_PROFILE_FUNCTION();
	}
} // namespace Azer