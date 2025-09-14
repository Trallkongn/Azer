#pragma once
#include <Azer/Core/Core.h>
#include <vector>
#include <Azer/Renderer/VertexArray.h>
#include <unordered_map>
#include <string>

namespace Azer {

	class VAOLibrary
	{
	public:
		static Ref<VertexArray> CreateCubeVAO(float size = 1.0f);
		static Ref<VertexArray> CreatePlaneVAO(float width = 1.0f, float depth = 1.0f);
		static Ref<VertexArray> CreateSphereVAO(float radius = 1.0f, int stacks = 16, int slices = 32);
		static Ref<VertexArray> CreateCylinderVAO(float radius = 0.5f, float height = 1.0f, int segments = 32);
	private:
		static Ref<VertexArray> CreateVAO(void* vertices, uint32_t size, void* indices, uint32_t count);
	};
}