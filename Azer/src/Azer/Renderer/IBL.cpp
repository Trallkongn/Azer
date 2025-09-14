#include "azpch.h"
#include "IBL.h"
#include "Renderer.h"
#include <Platform/OpenGL/IBL/OpenGLHDRtoCubeMap.h>
#include <Platform/OpenGL/IBL/OpenGLIrradianceMap.h>
#include <Platform/OpenGL/IBL/OpenGLPrefilterMap.h>
#include <Platform/OpenGL/IBL/OpenGLBRDFLUT.h>

namespace Azer {

	Ref<HDRtoCubeMap> HDRtoCubeMap::Create(
		uint32_t width, 
		uint32_t height, 
		const std::string& HDRpath, 
		Ref<VertexArray> vertexArray,
		const glm::mat4& captureProjection,
		const std::vector<glm::mat4>& captureViews
	)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLHDRtoCubeMap>(width, height, HDRpath, vertexArray,captureProjection,captureViews);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<IrradianceMap> IrradianceMap::Create(uint32_t width, uint32_t height,
		uint32_t skyboxID,
		Ref<VertexArray> vertexArray,
		const glm::mat4& captureProjection,
		const std::vector<glm::mat4>& captureViews
	)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIrradianceMap>(width, height, skyboxID, vertexArray, captureProjection, captureViews);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<PrefilterMap> PrefilterMap::Create(uint32_t miplevels, uint32_t width, uint32_t height,
		uint32_t skyboxID,
		Ref<VertexArray> vertexArray,
		const glm::mat4& captureProjection,
		const std::vector<glm::mat4>& captureViews
	)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLPrefilterMap>(miplevels,width,height,skyboxID,vertexArray,captureProjection,captureViews);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}

	Ref<BRDFLUT> BRDFLUT::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLBRDFLUT>(width,height);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}
	
}


