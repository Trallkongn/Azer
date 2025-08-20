#pragma once

#include <Azer/Core/Core.h>
#include <Azer/Renderer3D/PerspectiveGraphicCamera.h>

namespace Azer {

	class RendererPBR
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveGraphicCamera camera);
		static void EndScene();

		static void DrawHDR_Env(const glm::vec3& position, const glm::vec3& size);
	};
}