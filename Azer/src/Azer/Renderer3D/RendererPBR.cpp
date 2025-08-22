#include "azpch.h"
#include "RendererPBR.h"

#include <Azer/Renderer/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Renderer.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Renderer/IBL.h>

#include <Platform/OpenGL/IBL/PBR.h>

namespace Azer {

	struct RendererPBRStorage
	{
		Ref<HDRtoCubeMap> hMap;
		Ref<IrradianceMap> iMap;
		Ref<PrefilterMap>  pMap;
		Ref<BRDFLUT> brdflut;
		Ref<PBR> pbr;

		Ref<VertexArray> vertexArray;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 camPos;
	};

	static RendererPBRStorage* s_Data_PBR;

	void RendererPBR::BeginScene(PerspectiveGraphicCamera camera)
	{
		s_Data_PBR->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data_PBR->projectionMatrix = camera.GetProjectionMatrix();
		s_Data_PBR->viewMatrix = camera.GetViewMatrix();
		s_Data_PBR->camPos = camera.GetCamPos();
	}

	void RendererPBR::Init()
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

		s_Data_PBR->hMap = HDRtoCubeMap::Create(1024, 1024, "assets/textures/qwantani_dusk_2_puresky_4k.hdr", s_Data_PBR->vertexArray, m_CaptureProjection, m_CaptureViews);
		s_Data_PBR->hMap->BakeRender();

		s_Data_PBR->iMap = IrradianceMap::Create(32, 32, s_Data_PBR->hMap->GetRendererID(), s_Data_PBR->vertexArray, m_CaptureProjection,m_CaptureViews);

		s_Data_PBR->iMap->BakeRender();

		s_Data_PBR->pMap = PrefilterMap::Create(5,128, 128, s_Data_PBR->hMap->GetRendererID(), s_Data_PBR->vertexArray, m_CaptureProjection, m_CaptureViews);

		s_Data_PBR->pMap->BakeRender();

		s_Data_PBR->brdflut = BRDFLUT::Create(512, 512);
		s_Data_PBR->brdflut->BakeRender();

		s_Data_PBR->pbr = CreateRef<PBR>();
	}

	void RendererPBR::DrawHDR_Env(const glm::vec3& position, const glm::vec3& size)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));

		s_Data_PBR->hMap->RenderScene(s_Data_PBR->viewMatrix,s_Data_PBR->projectionMatrix);
		s_Data_PBR->pbr->Render(s_Data_PBR->iMap->GetRendererID(),s_Data_PBR->pMap->GetRendererID(),
			s_Data_PBR->brdflut->GetRendererID(),
			s_Data_PBR->camPos,transform,s_Data_PBR->ViewProjectionMatrix);
	}

	void RendererPBR::EndScene()
	{

	}

	void RendererPBR::Shutdown()
	{

	}
}