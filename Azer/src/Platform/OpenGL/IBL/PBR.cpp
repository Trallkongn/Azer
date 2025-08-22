#include "azpch.h"
#include "PBR.h"
#include <glad/glad.h>
#include <Azer/Renderer/Renderer.h>
#include <cstddef> // for offsetof

#include <Azer/FileSystem/SourceLoader.h>
#include <Azer/GLTF/GLTF.h>

namespace Azer {

	PBR::PBR()
	{
        m_PBR_shader = Shader::Create("assets/shaders/PBR/PBR_shader_with_no_tangent.glsl");

        auto& loader = SourceLoader::Instance();

        Ref<Asset> asset = loader.Load("assets/textures/gltf/oak_veneer_01_2k.gltf");
        
        auto& i = std::dynamic_pointer_cast<GLTF>(asset);

        m_Material = i->GetMaterials()[0];

        AZ_TRACE("GLTF UUID: {}", i->GetAssetID().ToString());

        m_VertexArray = VertexArray::Create();

        float cubeVertices[] = {
            // positions          // normals           // texcoords
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

            // Left face
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            // Right face
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

             // Bottom face
             -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
              0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
              0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
              0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
             -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

             // Top face
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        Ref<VertexBuffer> vbo = VertexBuffer::Create(cubeVertices,sizeof(cubeVertices));

        BufferLayout layout = {
            { ShaderDataType::Float3, "aPos", false, offsetof(Vertex, pos)},
            { ShaderDataType::Float3, "aNormal", false, offsetof(Vertex, normal)},
            { ShaderDataType::Float2, "aTexCoord", false, offsetof(Vertex, uv)}
        };
        vbo->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vbo);
	}

	PBR::~PBR()
	{

	}

    void PBR::Render(uint32_t iMap, uint32_t pMap, uint32_t brdfLUT,const glm::vec3& camPos, const glm::mat4& transform, const glm::mat4& viewProjectionMatrix)
    {
        m_PBR_shader->Bind();

        // ---------------------------
        // 设置矩阵
        // ---------------------------
        m_PBR_shader->SetFloat3("camPos", camPos);
        m_PBR_shader->SetMat4("u_Transform", transform);
        m_PBR_shader->SetMat4("u_ViewProjection", viewProjectionMatrix);

        // ---------------------------
        // 绑定 PBR 材质贴图
        // ---------------------------
        m_Material.m_AlbedoMap->Bind(0);
        m_PBR_shader->SetInt("albedoMap", 0);

        m_Material.m_MetallicRoughnessMap->Bind(1);
        m_PBR_shader->SetInt("metallicRoughnessMap", 1);

        m_Material.m_AoMap->Bind(2);
        m_PBR_shader->SetInt("aoMap", 2);

        glBindTextureUnit(3, iMap);
        m_PBR_shader->SetInt("irradianceMap", 3);

        glBindTextureUnit(4, pMap);
        m_PBR_shader->SetInt("prefilterMap", 4);

        glBindTextureUnit(5, brdfLUT);
        m_PBR_shader->SetInt("brdfLUT", 5);

        // ---------------------------
        // 点光源设置
        // ---------------------------
        m_PBR_shader->SetInt("lightCount", 4);
        m_PBR_shader->SetFloat3("lights[0].Position", glm::vec3(0.0f, 2.0f, 2.0f));
        m_PBR_shader->SetFloat3("lights[0].Color", glm::vec3(100.0f, 100.0f, 100.0f));
        m_PBR_shader->SetFloat3("lights[1].Position", glm::vec3(0.0f, -2.0f, -2.0f));
        m_PBR_shader->SetFloat3("lights[1].Color", glm::vec3(100.0f, 100.0f, 100.0f));
        m_PBR_shader->SetFloat3("lights[2].Position", glm::vec3(5.0f, -5.0f, -10.0f));
        m_PBR_shader->SetFloat3("lights[2].Color", glm::vec3(100.0f, 100.0f, 100.0f));
        m_PBR_shader->SetFloat3("lights[3].Position", glm::vec3(-5.0f, 5.0f,  10.0f));
        m_PBR_shader->SetFloat3("lights[3].Color", glm::vec3(100.0f, 100.0f, 100.0f));

        // ---------------------------
        // 绘制球体
        // ---------------------------

        m_VertexArray->Bind();
        RenderCommand::DrawArray(m_VertexArray,36);
        m_VertexArray->UnBind();
    }
}