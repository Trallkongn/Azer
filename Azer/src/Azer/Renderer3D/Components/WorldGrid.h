#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Renderer/VertexArray.h>
#include <Azer/Renderer/Buffer.h>
#include <Azer/Renderer/Shader.h>
#include <Azer/Renderer3D/PerspectiveGraphicCamera.h>

namespace Azer {

	class WorldGrid
	{
	public:
		WorldGrid(float halfExtent = 100.0f, float spacing = 10.0f);
		~WorldGrid();

		void Draw(PerspectiveGraphicCamera& camera);

	private:
		Ref<VertexArray> m_VertexArray;
		uint32_t m_LineCounts;
		Ref<Shader> m_Shader;
	};
}