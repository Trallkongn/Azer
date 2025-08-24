#include "azpch.h"
#include "Renderer3D.h"
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer/Buffer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Renderer/Renderer.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Mesh/MeshLibrary.h>
#include <Azer/Renderer/IBL.h>
#include <Azer/Material/Material.h>
#include <Azer/FileSystem/SourceLoader.h>
#include <Azer/GLTF/GLTF.h>


namespace Azer {

	struct RendererPBRStorage
	{
		Ref<HDRtoCubeMap> hMap;
		Ref<IrradianceMap> iMap;
		Ref<PrefilterMap>  pMap;
		Ref<BRDFLUT> brdflut;

		uint32_t iMapID;
		uint32_t pMapID;
		uint32_t brtflutID;

		Ref<VertexArray> vertexArray;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 camPos;

		Material material;
	};

	static RendererPBRStorage* s_Data_PBR;

	struct Renderer3DStorage
	{
		Ref<Shader> Common_Shader;
		Ref<Shader> PBR_shader;
		Ref<Shader> Toon_shader;

		Ref<VertexArray> VAO;

		std::vector<Ref<VertexArray>> VAOs;
		std::vector<Material> Materials;

		Ref<Texture2D> ramp;
	};

	static Renderer3DStorage* s_Data_3D;

	void Renderer3D::Init() {
		AZ_PROFILE_FUNCTION();

		s_Data_3D = new Renderer3DStorage;

		s_Data_3D->Common_Shader = Shader::Create("assets/shaders/ColorShader.glsl");
		s_Data_3D->PBR_shader = Shader::Create("assets/shaders/PBR/PBR_shader.glsl");
		s_Data_3D->Toon_shader = Shader::Create("assets/shaders/ToonShading/ToonShadingAdvanced.glsl");
		s_Data_3D->ramp = Texture2D::Create("assets/textures/ToonShading/ramp.png");
		s_Data_3D->VAO = VertexArray::Create();

		s_Data_3D->VAO->Bind();

		Ref<Asset> asset = SourceLoader::Instance().Load("assets/textures/glb/blue_archive_yurizono_seia.glb");

		auto& i = std::dynamic_pointer_cast<GLTF>(asset);

		auto& meshes = i->GetMeshs();
		
		for (int j = 0; j < meshes.size(); j++)
		{
			Ref<VertexArray> vao = VertexArray::Create();
			LoadMesh(vao, j, meshes[j]);
			s_Data_3D->VAOs.push_back(vao);
		}

		PBRInit();
	}
	
	void Renderer3D::BeginScene(PerspectiveGraphicCamera& camera) {
		AZ_PROFILE_FUNCTION();
		s_Data_3D->Common_Shader->Bind();
		s_Data_3D->Common_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data_3D->PBR_shader->Bind();
		s_Data_3D->PBR_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data_3D->Toon_shader->Bind();
		s_Data_3D->Toon_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data_PBR->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data_PBR->projectionMatrix = camera.GetProjectionMatrix();
		s_Data_PBR->viewMatrix = camera.GetViewMatrix();
		s_Data_PBR->camPos = camera.GetCamPos();

		RenderCommand::CullFace(false);
	}

	void Renderer3D::EndScene() {
		AZ_PROFILE_FUNCTION();
		RenderCommand::CullFace();
	}

	void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation)
	{
		AZ_PROFILE_FUNCTION();

		(s_Data_3D->Common_Shader)->Bind();
		(s_Data_3D->Common_Shader)->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1,0,0))*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0,1,0))*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0,0,1));
		(s_Data_3D->Common_Shader)->SetMat4("u_Transform", transform);

		for (auto& VAO : s_Data_3D->VAOs)
		{
			VAO->Bind();
			RenderCommand::DrawIndexed(VAO);
			VAO->UnBind();
		}
	}

	void Renderer3D::DrawEnv()
	{
		s_Data_PBR->hMap->RenderScene(s_Data_PBR->viewMatrix, s_Data_PBR->projectionMatrix);
	}

	void Renderer3D::DrawPBR(const glm::vec3& position,
		const glm::vec3& size, 
		const glm::vec3& rotation,
		float metalness, float ao, float roughness, float exposure)
	{
		RenderCommand::CullFace(false);

		s_Data_3D->PBR_shader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		(s_Data_3D->PBR_shader)->SetMat4("u_Transform", transform);

		// ---------------------------
		// 设置矩阵
		// ---------------------------
		s_Data_3D->PBR_shader->SetFloat3("camPos", s_Data_PBR->camPos);
		s_Data_3D->PBR_shader->SetMat4("u_Transform", transform);

		s_Data_3D->PBR_shader->SetInt("albedoMap", 0);
		s_Data_3D->PBR_shader->SetInt("metallicRoughnessMap", 1);
		s_Data_3D->PBR_shader->SetInt("aoMap", 2);
		s_Data_3D->PBR_shader->SetInt("irradianceMap", 3);
		s_Data_3D->PBR_shader->SetInt("prefilterMap", 4);
		s_Data_3D->PBR_shader->SetInt("brdfLUT", 5);

		RenderCommand::BindTexUnit(s_Data_PBR->iMapID, 3);
		RenderCommand::BindTexUnit(s_Data_PBR->pMapID, 4);
		RenderCommand::BindTexUnit(s_Data_PBR->brtflutID, 5);

		s_Data_3D->PBR_shader->SetFloat("u_MetallicFactor", metalness);
		s_Data_3D->PBR_shader->SetFloat("u_RoughnessFactor", roughness);
		s_Data_3D->PBR_shader->SetFloat("u_AOFactor", ao);
		s_Data_3D->PBR_shader->SetFloat("u_Exposure", exposure);

		// ---------------------------
		// 点光源设置
		// ---------------------------
		s_Data_3D->PBR_shader->SetInt("lightCount", 4);
		s_Data_3D->PBR_shader->SetFloat3("lights[0].Position", glm::vec3(0.0f, 2.0f, 2.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[0].Color", glm::vec3(100.0f, 100.0f, 100.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[1].Position", glm::vec3(0.0f, -2.0f, -2.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[1].Color", glm::vec3(100.0f, 100.0f, 100.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[2].Position", glm::vec3(5.0f, -5.0f, -10.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[2].Color", glm::vec3(100.0f, 100.0f, 100.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[3].Position", glm::vec3(-5.0f, 5.0f, 10.0f));
		s_Data_3D->PBR_shader->SetFloat3("lights[3].Color", glm::vec3(100.0f, 100.0f, 100.0f));


		for (int i = 0; i < s_Data_3D->VAOs.size(); i++)
		{
			auto& mat = s_Data_3D->Materials[i];
			auto& VAO = s_Data_3D->VAOs[i];

			mat.m_AlbedoMap->Bind(0);
			mat.m_MetallicRoughnessMap->Bind(1);
			mat.m_AoMap->Bind(2);

			VAO->Bind();
			RenderCommand::DrawIndexed(VAO);
			VAO->UnBind();
		}

		RenderCommand::CullFace();
	}

	void Renderer3D::DrawToon(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation)
	{
		RenderCommand::CullFace(false);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		s_Data_3D->Toon_shader->Bind();
		s_Data_3D->Toon_shader->SetMat4("u_Transform", transform);

		// ---------------------------
		// 设置矩阵
		// ---------------------------
		s_Data_3D->Toon_shader->SetFloat3("viewPos", s_Data_PBR->camPos);

		s_Data_3D->Toon_shader->SetInt("mainTexture", 0);
		s_Data_3D->Toon_shader->SetInt("specularRamp", 1);
		s_Data_3D->ramp->Bind(1);

		// ---------------------------
		// 点光源设置
		// ---------------------------
		s_Data_3D->Toon_shader->SetFloat3("lightPos", glm::vec3(0.0f, 0.0f, 1.0f));


		for (int i = 0; i < s_Data_3D->VAOs.size(); i++)
		{
			auto& mat = s_Data_3D->Materials[i];
			auto& VAO = s_Data_3D->VAOs[i];

			mat.m_AlbedoMap->Bind(0);

			VAO->Bind();
			RenderCommand::DrawIndexed(VAO);
			VAO->UnBind();
		}

		RenderCommand::CullFace();
	}

	void Renderer3D::PBRInit()
	{
		s_Data_PBR = new RendererPBRStorage;
		s_Data_PBR->vertexArray = VertexArray::Create();

		// 顶点数据 (位置)
		float vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		glm::mat4 m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		std::vector<glm::mat4> m_CaptureViews = {
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +X
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // -X
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)), // -Y
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +Z
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f))  // -Z
		};

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"}
		};

		vbo->SetLayout(layout);

		s_Data_PBR->vertexArray->AddVertexBuffer(vbo);

		s_Data_PBR->hMap = HDRtoCubeMap::Create(1024, 1024, "assets/textures/HDR/billiard_hall_4k.hdr", s_Data_PBR->vertexArray, m_CaptureProjection, m_CaptureViews);
		s_Data_PBR->hMap->BakeRender();

		s_Data_PBR->iMap = IrradianceMap::Create(32, 32, s_Data_PBR->hMap->GetRendererID(), s_Data_PBR->vertexArray, m_CaptureProjection, m_CaptureViews);

		s_Data_PBR->iMap->BakeRender();

		s_Data_PBR->pMap = PrefilterMap::Create(5, 128, 128, s_Data_PBR->hMap->GetRendererID(), s_Data_PBR->vertexArray, m_CaptureProjection, m_CaptureViews);

		s_Data_PBR->pMap->BakeRender();

		s_Data_PBR->brdflut = BRDFLUT::Create(512, 512);
		s_Data_PBR->brdflut->BakeRender();

		s_Data_PBR->iMapID = s_Data_PBR->iMap->GetRendererID();
		s_Data_PBR->pMapID = s_Data_PBR->pMap->GetRendererID();
		s_Data_PBR->brtflutID = s_Data_PBR->brdflut->GetRendererID();

		RenderCommand::SetViewport(0, 0, 1920, 1080);

		auto& loader = SourceLoader::Instance();
		Ref<Asset> asset = loader.Load("assets/textures/glb/blue_archive_yurizono_seia.glb");
		auto& i = std::dynamic_pointer_cast<GLTF>(asset);
 		auto& materials = i->GetMaterials();

		for (auto& mat : materials)
		{
			s_Data_3D->Materials.push_back(mat);
		}

		AZ_CORE_TRACE("GLTF UUID: {}", i->GetAssetID().ToString());
	}

	void Renderer3D::LoadMesh(Ref<VertexArray> vao, int index, const MeshData& mesh)
	{
		vao->Bind();

		std::vector<Vertex> vertices = mesh.vertices;
		std::vector<uint32_t> indices = mesh.indices;

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos", false, offsetof(Vertex,position)},
			{ ShaderDataType::Float3, "aNormal", false, offsetof(Vertex, normal)},
			{ ShaderDataType::Float2, "aTexCoord", false, offsetof(Vertex, texCoord)}
		};

		vbo->SetLayout(layout);

		vao->AddVertexBuffer(vbo);

		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), indices.size());

		vao->SetIndexBuffer(ibo);

		vao->UnBind();
	}


	void Renderer3D::Shutdown() {
		// Cleanup code for the 3D renderer
		AZ_PROFILE_FUNCTION();
	}
} // namespace Azer