#pragma once

#include <Azer/Core/Core.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Texture.h>
#include <vector>
#include <glm/glm.hpp>

namespace Azer {

	class IBL 
	{
	public:
		// Default virtual destructor.
		virtual ~IBL() = default;

		// To get IBL box width, it's usually the same with height.
		virtual uint32_t GetWidth() const = 0;

		//To get IBL box height, it's usually the same with width.
		virtual uint32_t GetHeight() const = 0;

		// To get the cube map renderer ID or BRDFLUT ID, that depends on your impl.
		virtual uint32_t GetRendererID() const = 0;

		// Bake scene.
		virtual void BakeRender() = 0;
	};

	class HDRtoCubeMap : public IBL
	{
	public:
		// Render scene.
		virtual void RenderScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const = 0;

		// Set a projection matrix to capture your scene during a framebuffer. In the same scene, it's usually the same.
		virtual void SetCaptureProjection(const glm::mat4& captureProjection) = 0;

		// Set some view matrixs to capture your scene during a framebuffer. In the same scene, it's usually the same.
		virtual void SetCaptureViews(const std::vector<glm::mat4>& captureViews) = 0;

		// To get the skybox vertexArray.
		virtual Ref<VertexArray> GetVertexArray() const = 0;

		// Create an impl.
		static Ref<HDRtoCubeMap> Create(uint32_t width,uint32_t height, 
			const std::string& HDRpath,
			Ref<VertexArray> vertexArray,
			const glm::mat4& captureProjection,
			const std::vector<glm::mat4>& captureViews
		);
	};

	class IrradianceMap : public IBL
	{
	public:
		// Set a projection matrix to capture your scene during a framebuffer. In the same scene, it's usually the same.
		virtual void SetCaptureProjection(const glm::mat4& captureProjection) = 0;

		// Set some view matrixs to capture your scene during a framebuffer. In the same scene, it's usually the same.
		virtual void SetCaptureViews(const std::vector<glm::mat4>& captureViews) = 0;

		// To get the skybox vertexArray.
		virtual Ref<VertexArray> GetVertexArray() const = 0;

		static Ref<IrradianceMap> Create(uint32_t width, uint32_t height, 
			uint32_t skyboxID, 
			Ref<VertexArray> vertexArray,
			const glm::mat4& captureProjection,
			const std::vector<glm::mat4>& captureViews
		);
	};

	class PrefilterMap : public IBL
	{
	public:
		// Set a projection matrix to capture your scene during a framebuffer. In the same scene, it's usually the same.
		virtual void SetCaptureProjection(const glm::mat4& captureProjection) = 0;

		// Set some view matrixs to capture your scene during a framebuffer. In the same scene, it's usually the same.
		virtual void SetCaptureViews(const std::vector<glm::mat4>& captureViews) = 0;

		virtual uint32_t GetMiplevels() const = 0;

		// To get the skybox vertexArray.
		virtual Ref<VertexArray> GetVertexArray() const = 0;

		static Ref<PrefilterMap> Create(uint32_t miplevels, uint32_t width, uint32_t height,
			uint32_t skyboxID,
			Ref<VertexArray> vertexArray,
			const glm::mat4& captureProjection,
			const std::vector<glm::mat4>& captureViews
		);
	};

	class BRDFLUT
	{
	public:
		virtual ~BRDFLUT() = default;

		virtual void BakeRender() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetRendererID() const = 0;

		static Ref<BRDFLUT> Create(uint32_t width, uint32_t height);
	};
}