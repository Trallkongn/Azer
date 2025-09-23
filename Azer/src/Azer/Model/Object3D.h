#pragma once
#include "Azer/Core/Core.h"
#include "GameObject.h"
#include "Azer/Mesh/Mesh.h"
#include "Azer/Material/Material.h"

namespace Azer {

	class Object3D : public GameObject
	{
	public:
		Object3D(const std::string& name, const Mesh& mesh, const Material& material)
			: GameObject(name), mesh(mesh), material(material)
		{

		}
		~Object3D() {}

		Mesh mesh;
		Material material;
	};

	class UploadObject3D : public GameObject
	{
	public:
		UploadObject3D(Ref<VertexArray> vao,const UploadMat& material)
			: GameObject(""), VAO(vao), material(material)
		{

		}
		~UploadObject3D() {}

		Ref<VertexArray> VAO;
		UploadMat material;
	};
}