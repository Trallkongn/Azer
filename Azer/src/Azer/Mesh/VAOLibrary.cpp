#include "azpch.h"
#include "VAOLibrary.h"

#include <Azer/Renderer/Buffer.h>
#include "MeshLibrary.h"

namespace Azer {

	Ref<VertexArray> VAOLibrary::CreateCubeVAO(float size)
	{
		auto& i = MeshLibrary::CreateCube(size);
		std::vector<Vertex> vertices = i.vertices;
		std::vector<uint32_t> indices = i.indices;
		return CreateVAO(vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), indices.size());
	}

	Ref<VertexArray> VAOLibrary::CreatePlaneVAO(float width, float depth)
	{
		auto& i = MeshLibrary::CreatePlane(width, depth);
		std::vector<Vertex> vertices = i.vertices;
		std::vector<uint32_t> indices = i.indices;
		return CreateVAO(vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), indices.size());
	}

	Ref<VertexArray> VAOLibrary::CreateSphereVAO(float radius, int stacks, int slices)
	{
		auto& i = MeshLibrary::CreateSphere(radius, stacks, slices);
		std::vector<Vertex> vertices = i.vertices;
		std::vector<uint32_t> indices = i.indices;
		return CreateVAO(vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), indices.size());
	}

	Ref<VertexArray> VAOLibrary::CreateCylinderVAO(float radius, float height, int segments)
	{
		auto& i = MeshLibrary::CreateCylinder(radius, height, segments);
		std::vector<Vertex> vertices = i.vertices;
		std::vector<uint32_t> indices = i.indices;
		return CreateVAO(vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), indices.size());
	}

	Ref<VertexArray> Azer::VAOLibrary::CreateVAO(void* vertices, uint32_t size, void* indices, uint32_t count)
	{
		Ref<VertexArray> vao = VertexArray::Create();

		vao->Bind();

		Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, size);

		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos", false, offsetof(Vertex,position)},
			{ ShaderDataType::Float3, "aNormal", false, offsetof(Vertex, normal)},
			{ ShaderDataType::Float2, "aTexCoord", false, offsetof(Vertex, texCoord)}
		};

		vbo->SetLayout(layout);

		vao->AddVertexBuffer(vbo);

		Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, count);

		vao->SetIndexBuffer(ibo);

		vao->UnBind();

		return vao;
	}
}


