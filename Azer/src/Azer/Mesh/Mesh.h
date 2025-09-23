#pragma once
#include "glm/glm.hpp"

namespace Azer {

	//----------------  前向声明  ----------------//
	struct Bone;
	struct MorphTarget;
	struct LODGroup;

	//----------------  顶点布局  ----------------//
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
		//glm::vec4 tangent;
		//glm::vec4 color{ 1.f };
	};

	//----------------  子网格  ----------------//
	struct SubMesh
	{
		uint32_t indexOffset = 0; // 相对于 Mesh::indices 的全局偏移
		uint32_t indexCount = 0;
		uint32_t materialIdx = 0; // 指向外部材质数组下标
		// TODO: AABB
	};

	class Mesh
	{
	public:
		// Core Data
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		// SubMesh
		std::vector<SubMesh> subMeshes;

		// TODO: template Skeleton and Morph and LOD and Bounding...
	};
}