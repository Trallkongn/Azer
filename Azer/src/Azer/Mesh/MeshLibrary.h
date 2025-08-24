#pragma once
#include <Azer/Core/Core.h>
#include <glm/glm.hpp>

namespace Azer {

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class MeshLibrary {
	public:
		static MeshData CreateCube(float size = 1.0f);
		static MeshData CreatePlane(float width = 1.0f, float depth = 1.0f);
		static MeshData CreateSphere(float radius = 1.0f, int stacks = 16, int slices = 32);
		static MeshData CreateCylinder(float radius = 0.5f, float height = 1.0f, int segments = 32);
	};
}