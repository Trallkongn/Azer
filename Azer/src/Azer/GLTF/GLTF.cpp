#include "azpch.h"
#include "GLTF.h"

#include <Azer/FileSystem/FileFormatRecognizer.h>

namespace Azer {

	GLTF::GLTF(const std::string& path)
	{
        FileFormat fmt = FileFormatRecognizer::Instance().Recognize(path);

        bool ret = false;
        if (fmt == FileFormat::GLTF)
        {
            ret = m_Loader.LoadASCIIFromFile(&m_Model, &m_Err, &m_Warn, path);
            m_FileFormat = FileFormat::GLTF;
        }  
        else if (fmt == FileFormat::GLB)
        {
            ret = m_Loader.LoadBinaryFromFile(&m_Model, &m_Err, &m_Warn, path);
            m_FileFormat = FileFormat::GLB;
        }
            
        if (!m_Warn.empty()) {
            AZ_CORE_WARN("Load glTF : {}", m_Warn);
        }
        if (!m_Err.empty()) {
            AZ_CORE_ERROR("Load glTF : {}", m_Err);
        }

        AZ_CORE_ASSERT(ret,"Failed to load glTF");

        LoadMesh();
        LoadMaterials();
	}

    GLTF::~GLTF()
    {

    }

    void GLTF::LoadMesh()
    {
        for (auto& mesh : m_Model.meshes)
        {
            const tinygltf::Primitive& prim = mesh.primitives[0];

            bool hasTangent = prim.attributes.find("TANGENT") != prim.attributes.end();

            // POSITION
            const tinygltf::Accessor& posAccessor = m_Model.accessors.at(prim.attributes.at("POSITION"));
            const tinygltf::BufferView& posView = m_Model.bufferViews.at(posAccessor.bufferView);
            const tinygltf::Buffer& posBuffer = m_Model.buffers.at(posView.buffer);
            const float* posData = reinterpret_cast<const float*>(&(posBuffer.data[posView.byteOffset + posAccessor.byteOffset]));

            // NORMAL
            const tinygltf::Accessor& normAccessor = m_Model.accessors.at(prim.attributes.at("NORMAL"));
            const tinygltf::BufferView& normView = m_Model.bufferViews.at(normAccessor.bufferView);
            const tinygltf::Buffer& normBuffer = m_Model.buffers.at(normView.buffer);
            const float* normData = reinterpret_cast<const float*>(&(normBuffer.data[normView.byteOffset + normAccessor.byteOffset]));

            // TEXCOORD_0
            const tinygltf::Accessor& uvAccessor = m_Model.accessors.at(prim.attributes.at("TEXCOORD_0"));
            const tinygltf::BufferView& uvView = m_Model.bufferViews.at(uvAccessor.bufferView);
            const tinygltf::Buffer& uvBuffer = m_Model.buffers.at(uvView.buffer);
            const float* uvData = reinterpret_cast<const float*>(&(uvBuffer.data[uvView.byteOffset + uvAccessor.byteOffset]));

            // TANGENT (optional)
            const float* tangentData = nullptr;
            if (hasTangent) {
                AZ_CORE_INFO("This mesh has Tangent");
                const tinygltf::Accessor& tangentAccessor = m_Model.accessors.at(prim.attributes.at("TANGENT"));
                const tinygltf::BufferView& tangentView = m_Model.bufferViews.at(tangentAccessor.bufferView);
                const tinygltf::Buffer& tangentBuffer = m_Model.buffers.at(tangentView.buffer);
                tangentData = reinterpret_cast<const float*>(&(tangentBuffer.data[tangentView.byteOffset + tangentAccessor.byteOffset]));
            }

            Primitive primitive;
            AABB bounds;

            // 初始化包围盒
            bounds.min = { std::numeric_limits<float>::max(),
                           std::numeric_limits<float>::max(),
                           std::numeric_limits<float>::max() };
            bounds.max = { std::numeric_limits<float>::lowest(),
                           std::numeric_limits<float>::lowest(),
                           std::numeric_limits<float>::lowest() };

            // 读取顶点数据
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

                if (hasTangent) {
                    v.tangent[0] = tangentData[i * 4 + 0];
                    v.tangent[1] = tangentData[i * 4 + 1];
                    v.tangent[2] = tangentData[i * 4 + 2];
                    v.tangent[3] = tangentData[i * 4 + 3];
                }

                // 更新 AABB
                for (int j = 0; j < 3; j++) {
                    bounds.min[j] = std::min(bounds.min[j], v.pos[j]);
                    bounds.max[j] = std::max(bounds.max[j], v.pos[j]);
                }

                primitive.vertices.push_back(v);
            }

            // 读取索引
            const tinygltf::Accessor& idxAccessor = m_Model.accessors.at(prim.indices);
            const tinygltf::BufferView& idxView = m_Model.bufferViews.at(idxAccessor.bufferView);
            const tinygltf::Buffer& idxBuffer = m_Model.buffers.at(idxView.buffer);

            if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                const unsigned short* buf = reinterpret_cast<const unsigned short*>(&(idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]));
                for (size_t i = 0; i < idxAccessor.count; ++i) primitive.indices.push_back(buf[i]);
            }
            else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                const unsigned int* buf = reinterpret_cast<const unsigned int*>(&(idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]));
                for (size_t i = 0; i < idxAccessor.count; ++i) primitive.indices.push_back(buf[i]);
            }
            else {
                std::cerr << "Unsupported index component type\n";
                return;
            }

            // 构建 Mesh
            Mesh myMesh;
            myMesh.id = UUID::GenerateV4();
            myMesh.name = mesh.name;
            myMesh.bounds = bounds;
            myMesh.primitives.push_back(std::move(primitive));

            AZ_CORE_INFO("Mesh loaded: {} (Vertices: {}, Indices: {})",
                myMesh.name.c_str(),
                myMesh.primitives[0].vertices.size(),
                myMesh.primitives[0].indices.size());

            m_Meshes.push_back(std::move(myMesh));

            AZ_CORE_INFO("Bounds: Min({}, {}, {}) Max({}, {}, {})",
                bounds.min[0], bounds.min[1], bounds.min[2],
                bounds.max[0], bounds.max[1], bounds.max[2]);
        }
    }

    // 工具函数：返回加载的纹理，如果不存在就返回默认纹理
    static Ref<Texture2D> GetOrDefaultTexture(
        const tinygltf::Model& model, int texIndex, Ref<Texture2D> defaultTex)
    {
        if (texIndex < 0 || texIndex >= model.textures.size())
            return defaultTex;

        const tinygltf::Texture& tex = model.textures[texIndex];
        if (tex.source < 0 || tex.source >= model.images.size())
            return defaultTex;

        const tinygltf::Image& image = model.images[tex.source];
        if (image.image.empty())
            return defaultTex;

        return Texture2D::Create(image.width, image.height, image.image.data());
    }

    // 创建默认纹理：白色/黑色/中性 normal
    static Ref<Texture2D> CreateDefaultTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
    {
        unsigned char pixel[4] = { r, g, b, a };
        return Texture2D::Create(1, 1, pixel);
    }

    void GLTF::LoadMaterials()
    {
        // 预先准备默认纹理
        Ref<Texture2D> defaultWhite = CreateDefaultTexture(255, 255, 255); // albedo
        Ref<Texture2D> defaultNormal = CreateDefaultTexture(128, 128, 255); // 法线贴图默认
        Ref<Texture2D> defaultBlack = CreateDefaultTexture(0, 0, 0);       // emissive
        Ref<Texture2D> defaultGray = CreateDefaultTexture(128, 128, 128); // metallicRoughness / AO

        std::vector<Material> materials;
        materials.reserve(m_Model.materials.size());

        for (const auto& mat : m_Model.materials) {
            Material material;

            // Albedo
            if (mat.values.find("baseColorTexture") != mat.values.end()) {
                int index = mat.values.at("baseColorTexture").TextureIndex();
                material.m_AlbedoMap = GetOrDefaultTexture(m_Model, index, defaultWhite);
            }
            else {
                material.m_AlbedoMap = defaultWhite;
            }

            // Normal
            if (mat.additionalValues.find("normalTexture") != mat.additionalValues.end()) {
                int index = mat.additionalValues.at("normalTexture").TextureIndex();
                material.m_NormalMap = GetOrDefaultTexture(m_Model, index, defaultNormal);
            }
            else {
                material.m_NormalMap = defaultNormal;
            }

            // MetallicRoughness
            if (mat.values.find("metallicRoughnessTexture") != mat.values.end()) {
                int index = mat.values.at("metallicRoughnessTexture").TextureIndex();
                material.m_MetallicRoughnessMap = GetOrDefaultTexture(m_Model, index, defaultGray);
            }
            else {
                material.m_MetallicRoughnessMap = defaultGray;
            }

            // Occlusion
            if (mat.additionalValues.find("occlusionTexture") != mat.additionalValues.end()) {
                int index = mat.additionalValues.at("occlusionTexture").TextureIndex();
                material.m_AoMap = GetOrDefaultTexture(m_Model, index, defaultGray);
            }
            else {
                material.m_AoMap = defaultGray;
            }

            // Emissive
            if (mat.additionalValues.find("emissiveTexture") != mat.additionalValues.end()) {
                int index = mat.additionalValues.at("emissiveTexture").TextureIndex();
                material.m_EmissiveMap = GetOrDefaultTexture(m_Model, index, defaultBlack);
            }
            else {
                material.m_EmissiveMap = defaultBlack;
            }

            materials.push_back(material);
        }

        m_Materials = std::move(materials);
    }

}