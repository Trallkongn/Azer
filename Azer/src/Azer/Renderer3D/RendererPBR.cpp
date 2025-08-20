#include "azpch.h"
#include "RendererPBR.h"

#include <Azer/Renderer/Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Renderer.h>
#include <Azer/Renderer/Texture.h>

#include <glad/glad.h>

#include <Platform/OpenGL/Material/OpenGLMaterial.h>

#include <Platform/OpenGL/IBL/EnvCubemap.h>
#include <Platform/OpenGL/IBL/IrradianceMap.h>
#include <Platform/OpenGL/IBL/PrefilterMap.h>
#include <Platform/OpenGL/IBL/BRDFLUT.h>
#include <Platform/OpenGL/IBL/PBR.h>

namespace Azer {

	struct RendererPBRStorage
	{
		Ref<EnvCubeMap> eMap;
		Ref<IrradianceMap> iMap;
		Ref<PrefilterMap>  pMap;
		Ref<BRDFLUT> brdflut;
		Ref<PBR> pbr;

		Ref<VertexArray> vertexArray;
		Ref<OpenGLMaterial> material;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 camPos;
	};

	static RendererPBRStorage* s_Data_PBR;

	// 对PBR进行渲染，需要在其它RendererAPI前调用
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

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"}
		};

		vbo->SetLayout(layout);

		s_Data_PBR->vertexArray->AddVertexBuffer(vbo);

		s_Data_PBR->material = CreateRef<OpenGLMaterial>("assets/textures/gltf/oak_veneer_01_2k.gltf");

		s_Data_PBR->eMap = CreateRef<EnvCubeMap>(1024, 1024, "assets/textures/qwantani_dusk_2_puresky_4k.hdr", s_Data_PBR->vertexArray);
		s_Data_PBR->eMap->BakeRender();

		s_Data_PBR->iMap = CreateRef<IrradianceMap>(32, 32, s_Data_PBR->eMap->GetCubeMap(), s_Data_PBR->vertexArray);

		s_Data_PBR->iMap->BakeRender();

		s_Data_PBR->pMap = CreateRef<PrefilterMap>(128, 128, 5, s_Data_PBR->eMap->GetCubeMap(), s_Data_PBR->vertexArray);

		s_Data_PBR->pMap->BakeRender();

		s_Data_PBR->brdflut = CreateRef<BRDFLUT>();

		s_Data_PBR->brdflut->BakeRender();

		s_Data_PBR->material->SetIrradianceMapTexture(s_Data_PBR->iMap->GetCubeMap());
		s_Data_PBR->material->SetPrefilterMapTexture(s_Data_PBR->pMap->GetCubeMap());
		s_Data_PBR->material->SetBrdfLUTTexture(s_Data_PBR->brdflut->GetRendererID());

		s_Data_PBR->pbr = CreateRef<PBR>(s_Data_PBR->material);
	}

	// 渲染一个天空盒
	void RendererPBR::DrawHDR_Env(const glm::vec3& position, const glm::vec3& size)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, size.z));

		s_Data_PBR->eMap->Render(s_Data_PBR->viewMatrix,s_Data_PBR->projectionMatrix);
		s_Data_PBR->pbr->Render(s_Data_PBR->camPos,transform,s_Data_PBR->ViewProjectionMatrix);
	}

	void RendererPBR::EndScene()
	{

	}

	void RendererPBR::Shutdown()
	{

	}
}