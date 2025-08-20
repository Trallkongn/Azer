#pragma once
#include <Azer/Core/Core.h>
#include <Platform/OpenGL/OpenGLTexture.h>
#include <vector>

namespace Azer {

	enum class TextureType
	{
		None = -1,
		// 2D texture
		albedoMap,					// 基础颜色（漫反射颜色）
		normalMap,					// 法线贴图，改变表面法向量以模拟细节
		metallicRoughnessMap,		// 金属度，0 = 非金属，1 = 金属 | 粗糙度，影响镜面反射光散射程度
		aoMap,						// 环境光遮蔽，用于增强阴影和凹凸感
		emissiveMap,				// 自发光
		// CubeMap texture	
		irradianceMap,				// 漫反射环境光
		prefilterMap,				// 镜面反射环境光
		brdfLUTTexture,				// 镜面反射积分查表，优化 Cook-Torrance BRDF
		Count
	};

	struct Vertex
	{
		float pos[3];
		float normal[3];
		float uv[2];
	};

	struct CustomPrimitive
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	struct CustomMesh
	{
		CustomPrimitive primitive;
	};

	class OpenGLMaterial
	{
	public:
		OpenGLMaterial(const std::string& gltfPath);
		~OpenGLMaterial();

		void GetMesh(const tinygltf::Model& model);

		const CustomMesh& GetCustomMesh() const { return m_Mesh; }

		bool HasTangent() const { return m_HasTangent; }

	public:
		static Ref<Texture2D> CreateFallbackTexture(unsigned char r, unsigned char g, unsigned char b)
		{
			unsigned char data[3] = { r, g, b };
			return CreateRef<OpenGLTexture2D>(1, 1, data);
		}

	public:
		Ref<Texture2D> GetAlbedoMapTexture() const { return m_albedoMap_texture; }
		Ref<Texture2D> GetNormalMapTexture() const { return m_normalMap_texture; }
		Ref<Texture2D> GetMetallicRoughnessMapTexture() const { return m_metallicRoughnessMap_texture; }
		Ref<Texture2D> GetAoMapTexture() const { return m_aoMap_texture; }
		Ref<Texture2D> GetEmissiveMapTexture() const { return m_emissiveMap_texture; }
		Ref<CubeMap> GetIrradianceMapTexture() const { return m_irradianceMap_texture; }
		Ref<CubeMap> GetPrefilterMapTexture() const { return m_prefilterMap_texture; }
		uint32_t GetBrdfLUTTexture() const { return m_brdfLUT_texture; }

	public:
		void SetIrradianceMapTexture(Ref<CubeMap>& texture) { m_irradianceMap_texture = texture; }
		void SetPrefilterMapTexture(Ref<CubeMap>& texture) { m_prefilterMap_texture = texture; }
		void SetBrdfLUTTexture(uint32_t rendererID) { m_brdfLUT_texture = rendererID; }

	private:
		std::string m_gltfPath;
		CustomMesh m_Mesh;
		bool m_HasTangent = false;

		Ref<Texture2D> m_albedoMap_texture;
		Ref<Texture2D> m_normalMap_texture;
		Ref<Texture2D> m_metallicRoughnessMap_texture;
		Ref<Texture2D> m_aoMap_texture;
		Ref<Texture2D> m_emissiveMap_texture;

		Ref<CubeMap> m_irradianceMap_texture;
		Ref<CubeMap> m_prefilterMap_texture;
		uint32_t m_brdfLUT_texture;
	};
}