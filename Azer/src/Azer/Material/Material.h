#pragma once
#include <Azer/Core/Core.h>
#include <Azer/FileSystem/Source/Image.h>
#include "Azer/Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Azer {

	struct Material
	{
		// Parameter
		glm::vec3 Albedo = glm::vec3(1.0f);
		float Metallic = 0.0f;
		float Roughness = 1.0f;
		float AO = 1.0f;
		glm::vec3 Emissive = glm::vec3(0.0f);

		// Textures
		ImageData AlbedoMap;
		ImageData NormalMap;
		ImageData MetallicRoughnessMap;
		ImageData AoMap;
		ImageData EmissiveMap;
	};

	struct UploadMat
	{
		Ref<Texture2D> AlbedoMap;
		Ref<Texture2D> NormalMap;
		Ref<Texture2D> MetallicRoughnessMap;
		Ref<Texture2D> AoMap;
		Ref<Texture2D> EmissiveMap;
	};
}