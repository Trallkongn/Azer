#include "azpch.h"
#include "OpenGLMaterial.h"
#include <Platform/OpenGL/OpenGLTexture.h>

namespace Azer {

    OpenGLMaterial::OpenGLMaterial(const std::string& gltfPath)
        : m_gltfPath(gltfPath)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err, warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, m_gltfPath);
        // 如果是 .glb 就用 LoadBinaryFromFile

        if (!warn.empty()) {
            AZ_CORE_WARN("Load glTF : {}", warn);
        }
        if (!err.empty()) {
            AZ_CORE_ERROR("Load glTF : {}", err);
        }
        if (!ret) {
            AZ_CORE_ERROR("Failed to load glTF");
            return;
        }

        GetMesh(model);

        // 输出每个网格中每个 primitive 使用的是哪个材质索引
        for (size_t meshIdx = 0; meshIdx < model.meshes.size(); ++meshIdx) {
            const auto& mesh = model.meshes[meshIdx];
            for (size_t primIdx = 0; primIdx < mesh.primitives.size(); ++primIdx) {
                int matIndex = mesh.primitives[primIdx].material;
                std::cout << "Mesh[" << meshIdx << "] Primitive[" << primIdx << "] uses material index " << matIndex;
                if (matIndex >= 0 && matIndex < (int)model.materials.size()) {
                    std::cout << " (\"" << model.materials[matIndex].name << "\")";
                }
                std::cout << "\n";
            }
        }

        for (size_t i = 0; i < model.materials.size(); i++) {

            const auto& mat = model.materials[i];
            std::cout << "Material[" << i << "] name = \"" << mat.name << "\"\n";

            // Base Color
            if (mat.pbrMetallicRoughness.baseColorTexture.index >= 0) {
                int texIdx = mat.pbrMetallicRoughness.baseColorTexture.index;
                std::cout << "  BaseColorTexture -> Texture[" << texIdx << "] name = \""
                    << model.textures[texIdx].name << "\"\n";
                m_albedoMap_texture = CreateRef<OpenGLTexture2D>(model, texIdx);
                AZ_CORE_INFO("Get baseColorTexture (Albedo) from glTF");
            }

            // Metallic-Roughness
            if (mat.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
                int texIdx = mat.pbrMetallicRoughness.metallicRoughnessTexture.index;
                std::cout << "  MetallicRoughnessTexture -> Texture[" << texIdx << "] name = \""
                    << model.textures[texIdx].name << "\"\n";
                m_metallicRoughnessMap_texture = CreateRef<OpenGLTexture2D>(model, texIdx);
                AZ_CORE_INFO("Get metallicRoughnessTexture from glTF");
            }

            // Normal Map
            if (mat.normalTexture.index >= 0) {
                int texIdx = mat.normalTexture.index;
                std::cout << "  NormalTexture -> Texture[" << texIdx << "] name = \""
                    << model.textures[texIdx].name << "\"\n";
                m_normalMap_texture = CreateRef<OpenGLTexture2D>(model, texIdx);
                AZ_CORE_INFO("Get normalTexture from glTF");
            }

            // Occlusion Map
            if (mat.occlusionTexture.index >= 0) {
                int texIdx = mat.occlusionTexture.index;
                std::cout << "  OcclusionTexture -> Texture[" << texIdx << "] name = \""
                    << model.textures[texIdx].name << "\"\n";
                m_aoMap_texture = CreateRef<OpenGLTexture2D>(model, texIdx);
                AZ_CORE_INFO("Get occlusionTexture (AO) from glTF");
            }

            // Emissive Map
            if (mat.emissiveTexture.index >= 0) {
                int texIdx = mat.emissiveTexture.index;
                std::cout << "  EmissiveTexture -> Texture[" << texIdx << "] name = \""
                    << model.textures[texIdx].name << "\"\n";
                m_emissiveMap_texture = CreateRef<OpenGLTexture2D>(model, texIdx);
                AZ_CORE_INFO("Get emissiveTexture (自发光) from glTF");
            }
        }

        if (!m_albedoMap_texture)
        {
            m_albedoMap_texture = CreateFallbackTexture(255, 255, 255); // 白色
            AZ_CORE_WARN("Didn't get albedoMap_texture from glTF, use default");
        }
            
        if (!m_metallicRoughnessMap_texture)
        {
            m_metallicRoughnessMap_texture = CreateFallbackTexture(0, 255, 0); // R=0, G=1, B=0 → 金属度=0 粗糙度=1
            AZ_CORE_WARN("Didn't get metallicRoughnessMap_texture from glTF, use default");
        }
            
        if (!m_aoMap_texture)
        {
            m_aoMap_texture = CreateFallbackTexture(255, 255, 255); // AO=1
            AZ_CORE_WARN("Didn't get aoMap_texture from glTF, use default");
        }
            
    }

    OpenGLMaterial::~OpenGLMaterial()
    {

    }

    void OpenGLMaterial::GetMesh(const tinygltf::Model& model)
    {
        const tinygltf::Mesh& mesh = model.meshes[0];
        const tinygltf::Primitive& prim = mesh.primitives[0];

        m_HasTangent = prim.attributes.find("TANGENT") != prim.attributes.end();

        // 读取 POSITION
        const tinygltf::Accessor& posAccessor = model.accessors.at(prim.attributes.at("POSITION"));
        const tinygltf::BufferView& posView = model.bufferViews.at(posAccessor.bufferView);
        const tinygltf::Buffer& posBuffer = model.buffers.at(posView.buffer);
        const float* posData = reinterpret_cast<const float*>(&(posBuffer.data[posView.byteOffset + posAccessor.byteOffset]));

        // 读取 NORMAL
        const tinygltf::Accessor& normAccessor = model.accessors.at(prim.attributes.at("NORMAL"));
        const tinygltf::BufferView& normView = model.bufferViews.at(normAccessor.bufferView);
        const tinygltf::Buffer& normBuffer = model.buffers.at(normView.buffer);
        const float* normData = reinterpret_cast<const float*>(&(normBuffer.data[normView.byteOffset + normAccessor.byteOffset]));

        // 读取 TEXCOORD_0
        const tinygltf::Accessor& uvAccessor = model.accessors.at(prim.attributes.at("TEXCOORD_0"));
        const tinygltf::BufferView& uvView = model.bufferViews.at(uvAccessor.bufferView);
        const tinygltf::Buffer& uvBuffer = model.buffers.at(uvView.buffer);
        const float* uvData = reinterpret_cast<const float*>(&(uvBuffer.data[uvView.byteOffset + uvAccessor.byteOffset]));

        const float* tangentData = nullptr;
        if (m_HasTangent) {
            AZ_CORE_INFO("Has Tangent");
            const tinygltf::Accessor& tangentAccessor = model.accessors.at(prim.attributes.at("TANGENT"));
            const tinygltf::BufferView& tangentView = model.bufferViews.at(tangentAccessor.bufferView);
            const tinygltf::Buffer& tangentBuffer = model.buffers.at(tangentView.buffer);
            tangentData = reinterpret_cast<const float*>(&(tangentBuffer.data[tangentView.byteOffset + tangentAccessor.byteOffset]));
        }

        for (size_t i = 0; i < posAccessor.count; ++i) {
            Vertex v;
            v.pos[0] = posData[i * 3 + 0];
            v.pos[1] = posData[i * 3 + 1];
            v.pos[2] = posData[i * 3 + 2];

            v.normal[0] = normData[i * 3 + 0];
            v.normal[1] = normData[i * 3 + 1];
            v.normal[2] = normData[i * 3 + 2];

            v.uv[0] = uvData[i * 2 + 0];
            v.uv[1] = uvData[i * 2 + 1];

            m_Mesh.primitive.vertices.push_back(v);
        }

        // 读取索引
        const tinygltf::Accessor& idxAccessor = model.accessors.at(prim.indices);
        const tinygltf::BufferView& idxView = model.bufferViews.at(idxAccessor.bufferView);
        const tinygltf::Buffer& idxBuffer = model.buffers.at(idxView.buffer);

        if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            const unsigned short* buf = reinterpret_cast<const unsigned short*>(&(idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]));
            for (size_t i = 0; i < idxAccessor.count; ++i) m_Mesh.primitive.indices.push_back(buf[i]);
        }
        else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            const unsigned int* buf = reinterpret_cast<const unsigned int*>(&(idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]));
            for (size_t i = 0; i < idxAccessor.count; ++i) m_Mesh.primitive.indices.push_back(buf[i]);
        }
        else {
            std::cerr << "Unsupported index component type\n";
            return;
        }
    }

}