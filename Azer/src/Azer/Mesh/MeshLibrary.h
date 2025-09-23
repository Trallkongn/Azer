#pragma once
#include <Azer/Core/Core.h>
#include <glm/glm.hpp>
#include "Mesh.h"

namespace Azer {

	class MeshLibrary {
	public:
		static Mesh CreateCube(float size = 1.0f);
		static Mesh CreatePlane(float width = 1.0f, float depth = 1.0f);
		static Mesh CreateSphere(float radius = 1.0f, int stacks = 16, int slices = 32);
		static Mesh CreateCylinder(float radius = 0.5f, float height = 1.0f, int segments = 32);
	};
}