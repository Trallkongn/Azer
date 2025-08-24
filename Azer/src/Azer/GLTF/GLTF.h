#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Mesh/MeshLibrary.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Material/Material.h>
#include <tiny_gltf.h>

#include <Azer/FileSystem/Asset.h>

namespace Azer {

	class GLTF : public Asset
	{
	public:
		GLTF(const std::string& path);
		~GLTF();

	private:
		void LoadMesh();
		void LoadMaterials();

		void DebugMeshMaterialBinding(const tinygltf::Model& model);


	public:
		const std::vector<MeshData>& GetMeshs() const { return m_Meshes; }
		const std::vector<Material>& GetMaterials() const { return m_Materials; }
		int GetMaterialCount() const { return m_Materials.size(); }

		FileFormat GetFileFormat() const override { return m_FileFormat; }

	private:
		tinygltf::Model m_Model;
		tinygltf::TinyGLTF m_Loader;
		std::string m_Err, m_Warn;
		std::vector<MeshData> m_Meshes;

		FileFormat m_FileFormat;

	private:
		std::vector<Material> m_Materials;
	};
}