#pragma once
#include <Azer/Core/Core.h>
#include <Azer/Mesh/MeshLibrary.h>
#include <Azer/Renderer/Texture.h>
#include <Azer/Material/Material.h>
#include <tiny_gltf.h>

#include "Azer/FileSystem/Asset.h"

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
		const std::vector<Mesh>& GetMeshs()			const { return m_Meshes; }
		const std::vector<Material>& GetMaterials() const { return m_Materials; }
		int GetMaterialCount()						const { return m_Materials.size(); }
		int GetMeshCount()							const { return m_Meshes.size(); }
	private:
		tinygltf::Model		m_Model;
		tinygltf::TinyGLTF	m_Loader;
		std::string			m_Err, m_Warn;

		std::vector<Mesh>	m_Meshes;
		std::vector<Material> m_Materials;
	};
}