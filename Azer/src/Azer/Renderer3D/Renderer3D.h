#pragma once

#include "Azer/Core/Core.h"

#include "PerspectiveGraphicCamera.h"

namespace Azer {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveGraphicCamera& camera);
		static void EndScene();

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, glm::vec4& color);
		static void DrawCubeWithEdge(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,const glm::vec4& edgeColor);

		static void DrawTextureCube(const glm::vec3& position, const glm::vec3& size);
	};
}