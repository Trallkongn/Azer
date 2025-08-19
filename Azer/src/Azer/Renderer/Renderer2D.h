#pragma once

#include "OrthoGraphicCamera.h"
#include "glm/glm.hpp"
#include "Texture.h"

namespace Azer {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthoGraphicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture);
	};
}