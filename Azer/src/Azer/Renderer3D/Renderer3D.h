#pragma once

#include "Azer/Core/Core.h"

#include "PerspectiveGraphicCamera.h"

#include <Azer/Mesh/MeshLibrary.h>

namespace Azer {

	class VertexArray;

	class Renderer3D
	{
	public:
		static void Init();

		static void Shutdown();

		static void BeginScene(PerspectiveGraphicCamera& camera);
		static void EndScene();

		static void DrawCursor(PerspectiveGraphicCamera& camera, const glm::vec3& centerPos, const glm::vec2& viewportSize);
		static void DrawWorldGrid(PerspectiveGraphicCamera& camera);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const glm::vec3& rotation);
		static void DrawEnv();
		static void DrawPBR(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, float metalness, float ao, float roughness, float exposure);
		static void DrawToon(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, float light_strength = 100.0f);

	private:
		static void PBRInit();

		static void LoadMesh(Ref<VertexArray> vao, int index, const MeshData& mesh);
	};
}