#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/Texture.h>
#include <glm/glm.hpp>

namespace Azer {

	enum class TextureType
	{
		None = -1,
		// 2D texture
		albedoMap,					// 基础颜色（漫反射颜色） // support tradition
		normalMap,					// 法线贴图，改变表面法向量以模拟细节  // support tradition
		metallicRoughnessMap,		// 金属度，0 = 非金属，1 = 金属 | 粗糙度，影响镜面反射光散射程度
		aoMap,						// 环境光遮蔽，用于增强阴影和凹凸感
		emissiveMap,				// 自发光 // support tradition
		Count
	};

	struct Material
	{
		// Parameter
		glm::vec3 m_Albedo = glm::vec3(1.0f);
		float m_Metallic = 0.0f;
		float m_Roughness = 1.0f;
		float m_AO = 1.0f;
		glm::vec3 m_Emissive = glm::vec3(0.0f);

		// Maps
		Ref<Texture2D> m_AlbedoMap;
		Ref<Texture2D> m_NormalMap;
		Ref<Texture2D> m_MetallicRoughnessMap;
		Ref<Texture2D> m_AoMap;
		Ref<Texture2D> m_EmissiveMap;
	};
}